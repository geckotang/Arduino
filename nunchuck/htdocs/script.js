$(function(){
  var ws;
  var socketServerURL = 'ws://localhost:8784';
  var $btn = $('.websocket .btn');
  var $ws_status = $('#ws-state');

  var $ball = $('#ball');

  if(ws == null || ws.readyState != 1){
    ws = new WebSocket(socketServerURL);
    $btn.val('close');
    ws.onopen = function(){
      $ws_status.text('websocket open');
    };
    ws.onclose = function(){
      $ws_status.text('websocket closed');
      $btn.val('open');
    };
    ws.onmessage = function(e){
      if (/X:/.test(e.data)) {
        var x = e.data.match(/\d+/) | 0;
        $ball.css('left', x);
      }
      if (e.data == 'Z:1') {
        console.log('Zボタンを押した！！！');
      } else if (e.data == 'C:1') {
        console.log('Cボタンを押した！！！');
      } else if (e.data == 'C:1, Z:1') {
        console.log('CとZボタンを押した！！！');
      }
    };
  } else {
    ws.close();
    $btn.val('open');
  }

});
