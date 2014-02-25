#!/usr/bin/ruby

require 'bundler'
require 'yaml'
require 'pp'
Bundler.require

#ファボられたらシリアルポートに0を送る

sp = SerialPort.new("/dev/cu.usbmodem1411", 9600, 8, 1, SerialPort::NONE)

stream_config = YAML.load_file(File.expand_path(File.dirname(__FILE__) + '/config.yml'))

UserStream.configure do |config|
  config.consumer_key = stream_config[:consumer_key]
  config.consumer_secret = stream_config[:consumer_secret]
  config.oauth_token = stream_config[:oauth_token]
  config.oauth_token_secret = stream_config[:oauth_token_secret]
end
#
client = UserStream.client
client.user do |status|
  if status.has_key? "event"
    if status.target.screen_name == "GeckoTang"
      case status.event.to_sym
      when :favorite
        # ファボ
        sp.puts "0"
      else
        # 実装されていないイベント
      end
    end
  end
end
sp.close
