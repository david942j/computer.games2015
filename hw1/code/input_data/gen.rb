
def itoa62(i)
  return ('0'.ord+i).chr if i<=9
  i-=10
  return ('a'.ord+i).chr  if i<26
  i-=26
  return ('A'.ord+i).chr if i<26
  return 0.chr;
end
def main(size=20)
  s=%x(curl http://cn.puzzle-nurikabe.com?size=4)
  ar = s.scan(/<td\s?>[0-9]+<\/td>|<td><img class="l" name="i[_\d]+" src="nun.gif"\s?\/?><\/td>/).map{|c| next '.' if c.match(/img/); next itoa62(c.scan(/\d+/).first.to_i) }
  puts size
  puts ar.join.scan(/.{#{size}}/)
  puts "id: "+s.scan(/<p>20x20 数墙 题号: ([\d,]+)<\/p>/)[0][0].split(',').join
end
main
