#!/usr/bin/ruby

require 'bundler'
Bundler.require
require 'yaml'
require 'pp'

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
  #if status.has_key? "text"
  #  if status.in_reply_to_screen_name == "GeckoTang"
  #    pp status.text
  #    pp status.user.screen_name
  #  end
  #end
  if status.has_key? "event"
    case status.event.to_sym
    when :favorite
      # ファボ
      pp "@#{status.source.screen_name} favorited", status.target_object.text
    when :unfavorite
      # アンファボ
      pp "@#{status.source.screen_name} unfavorited", status.target_object.text
    else
      # 実装されていないイベント
    end
  end
end
