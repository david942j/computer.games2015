
def main(size=20)
  s=%x(curl http://cn.puzzle-nurikabe.com?size=4)
  ar = s.scan(/<td >[0-9]*<\/td>|<td><img class="l" name="i[_\d]*" src="nun.gif" \/><\/td>/).map{|c| next '.' if c.match(/img/); next c.scan(/\d+/).first }
  puts size
  puts ar.join.scan(/.{#{size}}/)
  puts "id: "+s.scan(/<p>20x20 数墙 题号: ([\d,]+)<\/p>/)[0][0].split(',').join
end
main
