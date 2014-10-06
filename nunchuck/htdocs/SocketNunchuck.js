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
            button: {
              press_c: false,
              press_z: false,
              hold_c: false,
              hold_z: false
            }
          }
        }
        //ヌンチャクのジョイスティックと加速度センサを判別する
        //ev.data = {
        //  "x":125,
        //  "y":-34,
        //  "ax":-203.00,
        //  "ay":17.00,
        //  "az":135.00,
        //  "button": {
        //    "press_c": true,
        //    "press_z": false
        //    "hold_c": true,
        //    "hold_z": false
        //  }
        //}
        var chuck = JSON.parse(ev.originalEvent.data);
        ev.data = _.extend(ev.data, chuck);
        if (ev.data.button.press_c || ev.data.button.press_z) {
          //ボタンが押された時だけ実行する
          that.vent.trigger('nc:onBtnPressed', ev);
        }
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
