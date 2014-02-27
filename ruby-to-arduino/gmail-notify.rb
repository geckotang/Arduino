#!/usr/bin/ruby

require 'bundler'
require 'yaml'
require 'pp'
Bundler.require

# gmail setting

config = YAML.load_file(File.expand_path(File.dirname(__FILE__) + '/config_gmail.yml'))
gmail = Gmail.connect(config[:username], config[:password])
prev_unread = gmail.inbox.count(:unread)

# serial port setting

port_file = "/dev/cu.usbmodem14241"
baud_rate = 9600
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE
sp = SerialPort.new(port_file, baud_rate, data_bits, stop_bits, parity)

wait_time = 4

loop do
  unread = gmail.inbox.count(:unread)

  pp "未読をチェックしています"
  pp "以前の未読件数は#{prev_unread}"
  pp "現在の未読件数は#{unread}"

  if unread >= prev_unread && unread != 0
    sp.write "#{unread}"
  end
  prev_unread = unread
  sleep wait_time
end
sp.close
