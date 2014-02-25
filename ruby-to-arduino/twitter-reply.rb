#!/usr/bin/ruby

require 'bundler'
require 'yaml'
require 'pp'
Bundler.require

#replyがGeckoTang宛にあったらシリアルポートに0を送る

sp = SerialPort.new("/dev/cu.usbmodem1411", 9600, 8, 1, SerialPort::NONE)

stream_config = YAML.load_file(File.expand_path(File.dirname(__FILE__) + '/config.yml'))

UserStream.configure do |config|
  config.consumer_key = stream_config[:consumer_key]
  config.consumer_secret = stream_config[:consumer_secret]
  config.oauth_token = stream_config[:oauth_token]
  config.oauth_token_secret = stream_config[:oauth_token_secret]
end

client = UserStream.client
client.user do |status|
  if status.has_key? "text"
    if status.in_reply_to_screen_name == "GeckoTang"
      sp.puts "0"
    end
  end
end
sp.close
