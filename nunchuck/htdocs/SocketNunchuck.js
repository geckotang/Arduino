//SocketNunchuck
(function(EveEve, undefined){

  var ns = function(option){
    this.ws = {};
    this.vent = new EveEve;
    this.option = option || {
      wsURL: 'ws://localhost:8784'
    };
    return this;
  };

  ns.prototype.on = function(ev, callback) {
    this.vent.on(ev, callback);
    return this;
  };

  ns.prototype.once = function(ev, callback) {
    this.vent.once(ev, callback);
    return this;
  };

  ns.prototype.off = function(ev) {
    this.vent.off(ev);
    return this;
  };

  ns.prototype.trigger = function() {
    this.vent.trigger(arguments);
    return this;
  };

  ns.prototype.open = function() {
    var that = this;
    if(that.ws == null || that.ws.readyState != 1){
      that.ws = new WebSocket(that.option.wsURL);
      //Websocketにイベントを貼る
      that.ws.onopen = function(ev){
        that.vent.trigger('ws:onOpen', ev);
      };
      that.ws.onmessage = function(ev){
        var ev = {
          originalEvent: ev,
          data: {
            x: 0,
            y: 0,
            ax: 0,
            ay: 0,
            az: 0,
            key: ''
          }
        }
        //ヌンチャクのジョイスティックと加速度センサを判別する
        //ev.originalEvent.data = {"x":125,"y":-34,"ax":-203.00,"ay":17.00,"az":135.00}
        if (/^{\"x\":/.test(ev.originalEvent.data)) {
          var chuck = JSON.parse(ev.originalEvent.data);
          ev.data.x = chuck.x;
          ev.data.y = chuck.x;
          ev.data.ax = chuck.ax;
          ev.data.ay = chuck.ay;
          ev.data.az = chuck.az;
          that.vent.trigger('nc:onJoyXYChanged', {x: chuck.x, y: chuck.y});
          that.vent.trigger('nc:onAccelXYZChanged', {x: chuck.ax, y: chuck.ay, z: chuck.az});
        }
        //ヌンチャクのボタン入力を判別する
        //ev.originalEvent.data = '{"C":true, "Z":true}'
        //ev.originalEvent.data = '{"C":true, "Z":false}'
        //ev.originalEvent.data = '{"C":false, "Z":false}'
        if (/^{\"C\":/.test(ev.originalEvent.data)) {
          var keys = JSON.parse(ev.originalEvent.data);
          if (keys.C === true && keys.Z === true) {
            ev.data.key = 'CZ';
          } else if (keys.Z === true) {
            ev.data.key = 'Z';
          } else if (keys.C === true) {
            ev.data.key = 'C';
          }
        }
        that.vent.trigger('nc:onBtnPressed', ev.data.key);
        that.vent.trigger('ws:onMessage', ev);
      };
      that.ws.onclose = function(ev){
        that.vent.trigger('ws:onClose', ev);
      };
    } else {
      that.close();
      that.vent.trigger('ws:onClose', null);
    }
    return this;
  };

  ns.prototype.close = function() {
    var that = this;
    if (that.ws.close) {
      that.ws.close();
    }
    return this;
  };

  window.SocketNunchuck = ns;
}(window.EveEve, undefined));
