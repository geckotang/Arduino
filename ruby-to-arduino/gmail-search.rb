#!/usr/bin/ruby

require 'bundler'
require 'yaml'
require 'pp'
Bundler.require

# gmail setting

config = YAML.load_file(File.expand_path(File.dirname(__FILE__) + '/config_gmail.yml'))
gmail = Gmail.connect(config[:username], config[:password])
gmail.peek = true
prev_unread = gmail.inbox.count(:unread)

wait_time = 4

gmail.inbox.find(:read).each do |email|
  pp "件名  ：#{email.subject}"
  pp "日付  ：#{email.date}"
  pp "送信元：#{email.from}"
  pp "送信先：#{email.to}"
end

loop do
  unread = gmail.inbox.count(:unread)

  pp "未読をチェックしています"
  pp "以前の未読件数は#{prev_unread}"
  pp "現在の未読件数は#{unread}"

  sleep wait_time
end
