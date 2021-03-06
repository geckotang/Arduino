#!/usr/bin/ruby
# -*- coding: utf-8 -*-
require "MeCab"
require 'bundler'
require 'yaml'
require 'pp'
Bundler.require

def ruby_talk(in_str)
	#debug
	#in_str ="オープンソースカンファレンス 2012 Hiroshima http://www.ospn.jp/osc2012-hiroshima/ 10月20日 広島国際学院大学 中野キャンパス"

	#入力文字を一旦すべて全角にする
	in_str = Moji.han_to_zen(in_str)

	wk_str_output = ""
	sv_hinsi = ""
	sv_hinsi_1 = ""
	buf = ""
	tag_sw = false
	str_serial = ""

	#形態素解析
	node = MeCab::Tagger.new.parseToNode(in_str)
	while node
		#読みを付加したかのフラグ
		cnv_flg = false
		#MeCabのタイトル行及び文字種不明は無視
		if ((node.surface[0] == nil) || (Moji.type(node.surface) == nil)) then
	 		node = node.next
			next
		end
	#debug
	#	puts "#{node.surface}\t#{node.feature}"
	#形態素解析結果がcsv形式でかえるので分解
		str_csv = node.feature.split(",")
		yomi = str_csv[8]
		hinsi = str_csv[0]
		hinsi_1 = str_csv[1]
		str = node.surface
		#品詞が変わったか
		if ((sv_hinsi != hinsi) || (sv_hinsi_1 != hinsi_1)) then
			sv_hinsi = hinsi
			sv_hinsi_1 = hinsi_1
			if (tag_sw == true) then
				wk_str_output = wk_str_output + buf + ">/"
				buf = ""
				tag_sw = false
			end
		 end
		#いくつかのアルファベット固有名詞の読み
		if (((hinsi_1=="固有名詞") || (hinsi=="名詞" and hinsi_1=="一般")) and Moji.type?(str,Moji::ALPHA)) then
			#english_noun_list.txtはCSV形式で"intel","インテル"のようにする。
			File::open(File.dirname(__FILE__) + "/english_noun_list.txt") {|f|
				f.each {|line|
					tmp = line.split(',')
					if (Moji.zen_to_han(str).downcase == Moji.zen_to_han(tmp[0]).downcase) then
						yomi = tmp[1].chomp+"/"
						str = yomi
						cnv_flg = true
					end
				}
			}
		end
		#品詞分類１が数字で漢字以外又は文字タイプがアルファベット。ただし上で読みがなを変換している奴は除く
		if (((Moji.type?(str,Moji::ALPHA)) || ((hinsi_1 == "数") and (!Moji.type?(str,Moji::ZEN_KANJI)))) and (cnv_flg==false)) then
			if (tag_sw == false) then
				tag_sw = true
				if (Moji.type?(str,Moji::ALPHA)) then
					buf = "<ALPHA VAL=" + Moji.zen_to_han(str)
				elsif (hinsi_1 == "数") then	#if (Moji.type?(str,Moji::NUMBER)) then #ドットの解釈が記号か数字か文脈で判断する為未使用
					buf = "<NUM VAL=" + Moji.zen_to_han(str)
				end
			else
				buf = buf + Moji.zen_to_han(str)
				#タグ内の文字数が50文字を超えたら一旦タグを閉じる
				if (buf.length>50) then
					wk_str_output = wk_str_output + buf + ">\n"
					buf = ""
					tag_sw = false
				end
			end
		else
		#漢字やひらがななど
			if (tag_sw == true) then
				wk_str_output = wk_str_output + buf + ">/"
				buf = ""
				tag_sw = false
			end
			#品詞分類が記号以外も読みが不明なものを補足するようにした。
			#記号の読みを補足する。（MeCabの辞書の一部記号は読みのところに記号がそのまま入っているものがある）
			#アクセント記号は諦めた。
			#読めない記号は無視
			File::open(File.dirname(__FILE__) + "/sign_list.txt") {|f|
				f.each {|line|
					tmp = line.split(',')
					if (Moji.zen_to_han(str).downcase == Moji.zen_to_han(tmp[0]).downcase) then
						yomi = tmp[1].chomp + "/"
						str = yomi
					end
				}
			}
			#特殊なものを変換
			if(str=="。")then
				yomi="."
			end
			if(str=="、")then
				yomi=","
			end
			#読めない記号はスペースに置換
			rep_chars=[" ","!",";","_","'","~"]
			if (rep_chars.include?(Moji.zen_to_han(str)))then
				yomi = " "
			end
			if((yomi!=nil) and (!Moji.type?(yomi,Moji::KANA)))then
				yomi = " "
			end
			#MeCabの辞書に載っていなくて、読みが不明なカタカナ、漢字はkakasiに任せてみる

			if(((yomi=="")||(yomi==nil)) and ((Moji.type?(str,Moji::ZEN_KANJI))||(Moji.type?(str,Moji::KANA)))) then
				yomi = str
			end
			#kakasiがeucじゃないと動かないので変換
			#kakasiで漢字かな混じり文をローマ字に変換。読みを変換している点注意(喋らすため）
			wk_str_output = wk_str_output + cmd_kakasi(yomi)
			#^は長音記号にしてみる
			wk_str_output = wk_str_output.gsub(/\^/,"-")
		end
		#句点があればそこで改行
		if (str == ".") || (str==",") then
			str_serial = str_serial + wk_str_output + "\n"
			wk_str_output = ""
		end
		#長すぎる音声記号は改行
		if (wk_str_output.length>100) then
			str_serial = str_serial + wk_str_output + "\n"
			wk_str_output = ""
		end
	#
		node = node.next
	end

	#最後のタグを閉じる処理
	 if (tag_sw == true) then
		wk_str_output = wk_str_output + buf + ">/"
		buf = ""
		tag_sw = false
	 end

	str_serial  = str_serial + wk_str_output
end

def cmd_kakasi(str)
  result = open("| echo " + str + " | kakasi -Ha -Ka -Ja -Ea -ka -ja -i utf-8 -o utf-8")
  return result.gets.sub(/\n/,"")
end

pp cmd_kakasi('日本語カナ、unkoです。');
pp ruby_talk("geckotang");
