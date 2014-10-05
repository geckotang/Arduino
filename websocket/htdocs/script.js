$(function(){
  var ws;
  var $btn = $('.websocket .btn');
  var $addr = $('#addr');
  var $led_on = $('.led-on');
  var $led_off = $('.led-off');
  var $ws_status = $('#ws-state');

  $btn.click(function(){
    if(ws == null || ws.readyState != 1){
      ws = new WebSocket($addr.val());
      $addr.attr('disabled','disabled');
      $btn.val('close');
      ws.onopen = function(){
        $ws_status.text('websocket open');
      };
      ws.onclose = function(){
        $ws_status.text('websocket closed');
        $btn.val('open');
        $addr.attr('disabled',null);
      };
      ws.onmessage = function(e){
        console.log(e);
      };
    } else {
      ws.close();
      $btn.val('open');
      $addr.attr('disabled',null);
    }
  });

  //Turn on the LED
  $led_on.click(function(){
    ws.send('o');
  });

  //Turn off the LED
  $led_off.click(function(){
    ws.send('x');
  });

});
