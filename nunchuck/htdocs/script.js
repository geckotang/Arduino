$(function(){

  var $ws_status = $('#ws-state');
  var $ball = $('#ball');
  var $btn_status = $('.nunchuck__state-btn');
  var $debuginfo = $('.nunchuck__debuginfo');
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
  //ev.originalEvent: Websocket MessageEvent
  //ev.data.x: Nunchuck Joy X
  //ev.data.y: Nunchuck Joy Y
  //ev.data.ax: Nunchuck Accel X
  //ev.data.ay: Nunchuck Accel Y
  //ev.data.az: Nunchuck Accel Z
  //ev.data.button.press_c: Press C
  //ev.data.button.press_z: Press Z
  //ev.data.button.hold_c: Hold C
  //ev.data.button.hold_z: Hold Z
  socketNunchuck.on('ws:onMessage', function(ev){
    var result = [];
    var chuck = ev.data;
    $ball.css({
      top: chuck.y * -1,
      left: chuck.x
    });
    result.push(
      'X: ' + chuck.x,
      'Y: ' + chuck.y,
      '加速センサX軸: ' + chuck.ax,
      '加速センサY軸: ' + chuck.ay,
      '加速センサZ軸: ' + chuck.az,
      'Cボタン押した: ' + chuck.button.press_c,
      'Zボタン押した: ' + chuck.button.press_z,
      'Cボタンホールド: ' + chuck.button.hold_c,
      'Zボタンホールド: ' + chuck.button.hold_z
    );
    $debuginfo.html(result.join('\n'));
  });

  //ボタンが何かしら押された時のイベント
  var pressedTimer;
  socketNunchuck.on('nc:onBtnPressed', function(ev){
    clearTimeout(pressedTimer);
    var chuck = ev.data;
    var button = chuck.button;
    if (button.press_c && button.press_z) {
      $btn_status.text('CZボタンが押されました');
    } else if (button.press_c) {
      $btn_status.text('Cボタンが押されました');
    } else if (button.press_z) {
      $btn_status.text('Zボタンが押されました');
    }
    pressedTimer = setTimeout(function(){
      $btn_status.text('');
    }, 100);
  });

  socketNunchuck.open();

});
