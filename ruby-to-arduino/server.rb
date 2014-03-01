# encoding: utf-8
require "cgi"
require 'webrick'
require 'erb'
require 'pp'
require 'bundler'
Bundler.require

## serialport
port_file = "/dev/cu.usbmodem14241"
baud_rate = 9600
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE
sp = SerialPort.new(port_file, baud_rate, data_bits, stop_bits, parity)

## twitter
stream_config = YAML.load_file(File.expand_path(File.dirname(__FILE__) + '/config.yml'))
twitter = Twitter::REST::Client.new do |config|
  config.consumer_key        = stream_config[:consumer_key]
  config.consumer_secret     = stream_config[:consumer_secret]
  config.access_token        = stream_config[:oauth_token]
  config.access_token_secret = stream_config[:oauth_token_secret]
end

srv = WEBrick::HTTPServer.new({
  :DocumentRoot => './',
  :BindAddress => '0.0.0.0',
  :Port => 8888})

sitename = "CallGrid";
usernames = {
  "geckotang"=> "ge'/kotann",
  "yomotsu"  => "yo'mo_tsu",
  "5509"     => "no'ri",
  "nakajmg"  => "ji'/ma'/gu'",
  "hokaccha" => "hokacha.",
  "kyosuke"  => "kyo-_suke",
  "saucerjp" => "a'ria/ha'nn",
  "takazudo" => "zu'do",
  "tacamy"   => "takami-'",
  "ykhs"     => "yukki'/-'",
  "soto"     => "soto"
}

# 素材場所
srv.mount("/asset", WEBrick::HTTPServlet::FileHandler, "./asset")

# index
srv.mount_proc '/' do |req, res|
  contents = <<EOS
<!DOCTYPE html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=yes">
    <meta name="format-detection" content="email=no">
    <meta name="format-detection" content="telephone=no">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta charset="utf-8">
    <script src="/asset/js/script.js"></script>
    <link rel="stylesheet" type="text/css" href="/asset/css/style.css" media="all">
    <title><%= sitename %></title>
  </head>
  <body>
    <div class="mod-header">
      <h1 class="mod-header-heading"><%= sitename %></h1>
      <p class="mod-header-text">
        ピクセルグリッドのメンバーに進捗を聞いたりできます
      </p>
    </div>
    <form class="mod-block  mod-form" action="/call">
      <h2 class="mod-heading">進捗を聞く</h2>
      <div class="mod-form-item">
        <label class="mod-form-checkbox"><input type="checkbox" name="flg_tweet" id="flg_tweet"><span></span>ツイートでも呼び出す</label>
      </div>
      <div class="mod-form-item">
        <label class="mod-form-selectWrap" for="username">
          <select class="mod-form-select" id="username" name="username">
            <optgroup>
              <%- usernames.each do |key,value| -%>
                <option value="<%= key %>"><%= key %></option>
              <%- end -%>
            </optgroup>
          </select>
        </label>
        を
        <button class="mod-btn  mod-btn--type-submit" type="submit">呼び出す</button>
      </div>
    </form>
    <form id="sayTimeForm" target="iframe-time" class="mod-block  mod-form" action="/time">
      <h2 class="mod-heading">現在時刻</h2>
      <div class="mod-form-item">
        <button id="sayTimeBtn" class="mod-btn  mod-btn--type-submit" type="submit">今の時間を発声する</button>
      </div>
    </form>
    <iframe id="iframe-time" src="" frameborder="0" width="0" height="0"></iframe>
    <form id="sayTextForm" target="iframe-time" class="mod-block  mod-form" action="/text">
      <h2 class="mod-heading">好きな言葉をしゃべらせる</h2>
      <div class="mod-form-item">
        <div class="mod-text">
          <a href="http://www.a-quest.com/demo/pico_kanji2roman.html" target="_blank">音声記号列生成 Webサービス</a>で生成してください。
        </div>
        <input id="sayTextInput" class="mod-form-input" type="text" placeholder="音声記号で入力してね！" name="text">
        <button id="sayTextBtn" class="mod-btn  mod-btn--type-submit" type="submit">発声させる</button>
      </div>
    </form>
    <iframe id="iframe-text" src="" frameborder="0" width="0" height="0"></iframe>
  <body>
</html>
EOS
  res.body = ERB.new(contents, nil, '-').result(binding)
end

srv.mount_proc '/call' do |req, res|
  pp username = req.query['username']
  pp flg_tweet = req.query['flg_tweet']
  pronunciation = usernames[username]
  #ツイートする
  if flg_tweet == "on"
    twitter.update("@"+username+" 進捗どうですか？")
  end
  #呼び出し
  sp.puts pronunciation + "/sann/ shi'nnchoku/do'-de_suka.";
  #HTML表示
  contents =<<EOS
<%# encoding: utf-8%>
<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=yes">
    <meta name="format-detection" content="email=no">
    <meta name="format-detection" content="telephone=no">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta charset="utf-8">
    <link rel="stylesheet" type="text/css" href="/asset/css/style.css" media="all">
    <title><%= sitename %></title>
  </head>
  <body>
    <div class="mod-header">
      <h1 class="mod-header-heading"><%= sitename %></h1>
    </div>
    <div class="mod-block">
      <p class="mod-text"><b><%= username %></b>さんに進捗を聞きました</p>
      <a class="mod-btn  mod-btn--type-back" href="/">戻る</a>
    </div>
  </body>
</html>
EOS
  res.body = ERB.new(contents, nil, '-').result(binding)
end

srv.mount_proc '/time' do |req, res|
  time = Time.now
  say  = "ge'nnzaino/jikannwa <NUMK VAL=#{time.hour} COUNTER=ji>/<NUMK VAL=#{time.min} COUNTER=funn>/<NUMK VAL=#{time.sec} COUNTER=byo->de_su."
  sp.puts say;
  isBusy = true;
  i = 0
  while isBusy
    if sp.gets then
      isBusy = false
    end
    sleep 1
  end
  contents =<<EOS
<%# encoding: utf-8%>
<body> <script> window.parent.isNotBusy(); </script> </body>
EOS
  res.body = ERB.new(contents, nil, '-').result(binding)
end


srv.mount_proc '/text' do |req, res|
  say  = CGI.unescape(req.query['text'])
  pp "output: " + say
  sp.puts say;
  isBusy = true;
  while isBusy
    gets = sp.gets
    if gets then
      pp "input: " + gets
      isBusy = false
    end
    sleep 1
  end
  contents =<<EOS
<%# encoding: utf-8%>
<body> <script> window.parent.isNotBusy(); </script> </body>
EOS
  res.body = ERB.new(contents, nil, '-').result(binding)
end


trap("INT"){ srv.shutdown } # 端末から割り込みキーを受ける
srv.start
sp.close
