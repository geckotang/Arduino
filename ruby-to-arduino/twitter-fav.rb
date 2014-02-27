#!/usr/bin/ruby

require 'bundler'
require 'yaml'
require 'pp'
Bundler.require

#ファボられたらシリアルポートに0を送る

port_file = "/dev/cu.usbmodem14241"
baud_rate = 9600
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE
sp = SerialPort.new(port_file, baud_rate, data_bits, stop_bits, parity)

stream_config = YAML.load_file(File.expand_path(File.dirname(__FILE__) + '/config.yml'))

UserStream.configure do |config|
  config.consumer_key = stream_config[:consumer_key]
  config.consumer_secret = stream_config[:consumer_secret]
  config.oauth_token = stream_config[:oauth_token]
  config.oauth_token_secret = stream_config[:oauth_token_secret]
end

pp Romaji.kana2romaji "geckotang"
pp Romaji.romaji2kana "geckotang"

client = UserStream.client
client.user do |status|
  if status.has_key? "event"
    if status.target.screen_name == "GeckoTang"
      case status.event.to_sym
      when :favorite
        # ファボ
        pp s_name = 'glatyou'#Moji.downcase(status.source.screen_name)
        pp s_name_hiragana = s_name.roman_to_hiragana
        pp s_name_roman = s_name_hiragana.hiragana_to_roman
        sp.puts Moji.downcase(status.source.screen_name)
      else
        # 実装されていないイベント
      end
    end
  end
end
sp.close
