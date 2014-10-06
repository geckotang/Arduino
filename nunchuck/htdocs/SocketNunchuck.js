//SocketNunchuck
(function(EveEve, undefined){

  var ns = function(option){
    this.ws = {};
    this._chuck = {};
    this.joyAngle = 0;
    this.isActive = false;
    this.isCHeld = false;
    this.isZHeld = false;
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
        that.isActive = true;
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
        //  "joyDeg":90,
        //  "button":{
        //    "press_c": true,
        //    "press_z": false
        //    "hold_c": true,
        //    "hold_z": false
        //  }
        //}
        var chuck = JSON.parse(ev.originalEvent.data);
        ev.data = _.extend(ev.data, chuck);
        that._chuck = _.extend({}, ev.data);
        if (ev.data.button.press_c || ev.data.button.press_z) {
          //ボタンが押された時だけ実行する
          that.vent.trigger('nc:onBtnPressed', ev);
        }
        //Cがホールドされているかどうか
        that.isCHeld = ev.data.button.hold_c;
        //Zがホールドされているかどうか
        that.isZHeld = ev.data.button.hold_z;
        //角度を入れる
        var normalzedPos = that.getJoyNormalizedPosition();
        that.joyAngle = that.getJoyAngle(normalzedPos.x, normalzedPos.y);
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
      that.isActive = false;
    }
    return this;
  };

  //ジョイスティックの角度を取得する
  ns.prototype.getJoyAngle = function(JoyNormalizedPosX, JoyNormalizedPosY) {
    var that = this;
    var angle = Math.atan( JoyNormalizedPosY / JoyNormalizedPosX );
    if ( JoyNormalizedPosX === 0 && JoyNormalizedPosY === 0 ) {
      return this.angle;
    }
    if ( 0 > JoyNormalizedPosX && 0 <= JoyNormalizedPosY ) {
      //the second quadrant
      angle += Math.PI;
    } else if ( 0 > JoyNormalizedPosX && 0 > JoyNormalizedPosY ) {
      //the third quadrant
      angle += Math.PI;
    } else if ( 0 <= JoyNormalizedPosX && 0 > JoyNormalizedPosY ) {
      //the fourth quadrant
      angle += Math.PI * 2;
    }
    return angle;
  };

  //walkthroughのための位置変換
  ns.prototype.getJoyNormalizedPosition = function() {
    var that = this;
    if (that._chuck) {
      return getNormalizedPosition(that._chuck.x, that._chuck.y);
    } else {
      return {x:0, y:0};
    }
  };

  // wiiInputX <number> left 30 to 125 to 220 right
  // wiiInputY <number> down -130 to 35 to 60 up
  // this will return -1 to 1
  // thanks @yomotsu
  function getNormalizedPosition( wiiInputX, wiiInputY ) {
   var x = -( 125 - wiiInputX ) / 95;
   var y = ( 35 + wiiInputY )   / 95;
   return { x: x, y: y };
  };

  window.SocketNunchuck = ns;
}(window.EveEve, undefined));
