#!/usr/bin/ruby

require 'bundler'
require 'yaml'
require 'pp'
Bundler.require

stream_config = YAML.load_file(File.expand_path(File.dirname(__FILE__) + '/config.yml'))

client = Twitter::REST::Client.new do |config|
  config.consumer_key        = stream_config[:consumer_key]
  config.consumer_secret     = stream_config[:consumer_secret]
  config.access_token        = stream_config[:oauth_token]
  config.access_token_secret = stream_config[:oauth_token_secret]
end

#最新ツイートをふぁぼる
latestTweetID = client.home_timeline().first.id
client.favorite(latestTweetID)
