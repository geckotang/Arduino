# encoding: utf-8
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
    <link rel="stylesheet" type="text/css" href="/asset/css/style.css" media="all">
    <title><%= sitename %></title>
  </head>
  <body>
    <div class="mod-header">
      <h1><%= sitename %></h1>
    </div>
    <p class="mod-text">
      ピクセルグリッドのメンバーの進捗を聞きます
    </p>
    <form class="mod-form" action="/call">
      <div class="mod-form-item">
        <select class="mod-form-select" id="username" name="username">
          <%- usernames.each do |key,value| -%>
            <option value="<%= key %>"><%= key %></option>
          <%- end -%>
        </select>
        を
        <button type="submit">呼び出す</button>
      </div>
      <div class="mod-form-item">
        <label><input type="checkbox" name="flg_tweet" id="flg_tweet"> ツイートする</label>
      </div>
    </form>
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
      <h1><%= sitename %></h1>
    </div>
    <p class="mod-text"><%= username %>を呼び出しました。</p>
    <a class="mod-btn  mod-btn--type-back" href="/">戻る</a>
  </body>
</html>
EOS
  res.body = ERB.new(contents, nil, '-').result(binding)
end

trap("INT"){ srv.shutdown } # 端末から割り込みキーを受ける
srv.start
sp.close
