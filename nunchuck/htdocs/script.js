$(function(){

  var $ws_status = $('#ws-state');
  var $ball = $('#ball');
  var $btn_status = $('.nunchuck__state-btn');
  var $joyX_status = $('.nunchuck__state-joyX');
  var $joyY_status = $('.nunchuck__state-joyY');
  var $accelX_status = $('.nunchuck__state-accelX');
  var $accelY_status = $('.nunchuck__state-accelY');
  var $accelZ_status = $('.nunchuck__state-accelZ');
  var socketNunchuck = new SocketNunchuck({
    wsURL: 'ws://localhost:8784'
  });

  //Websocketが開いた時
  socketNunchuck.on('ws:onOpen', function(){
    $ws_status.text('websocket open');
  });

  //Websocketが閉じた時
  socketNunchuck.on('ws:onClose', function(){
    $ws_status.text('websocket closed');
  });

  //何かしらメッセージを受け取った時
  socketNunchuck.on('ws:onMessage', function(ev){
    //ev.originalEvent: Websocket MessageEvent
    //ev.data.x: Nunchuck Joy X
    //ev.data.y: Nunchuck Joy Y
    //ev.data.ax: Nunchuck Accel X
    //ev.data.ay: Nunchuck Accel Y
    //ev.data.az: Nunchuck Accel Z
    //ev.data.key: Nunchuck Pressed Key
  });

  //ジョイスティックが変化した時
  socketNunchuck.on('nc:onJoyXYChanged', function(pos){
    $ball.css({
      top: pos.y * -1,
      left: pos.x
    });
    $joyX_status.text(pos.x);
    $joyY_status.text(pos.y);
  });

  //ヌンチャクの加速度が変化した時
  socketNunchuck.on('nc:onAccelXYZChanged', function(accel){
    $accelX_status.text(accel.x);
    $accelY_status.text(accel.y);
    $accelZ_status.text(accel.z);
  });

  //ボタンが何かしら押された時のイベント
  socketNunchuck.on('nc:onBtnPressed', function(key){
    if (key) {
      $btn_status.text(key+'ボタンが押されました');
    } else {
      $btn_status.text('');
    }
  });

  socketNunchuck.open();

});
