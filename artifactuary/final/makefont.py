char_width = 5
char_height = 8

for c in range(128 - 32):
  print 'CHAR %d ("%s")' % (c + 32, chr(c + 32) if c + 32 != ord('"') and c + 32 != ord('\\') else '\\' + chr(c + 32))
  for x in range(char_height):
    print '.' * char_width
  print

"""
fa = [[0] * 8] * 96
for c in chars:
  newc = []
  for x in c[1]:
    newc.append(
      (0x01 if x & 0x80 != 0 else 0) |
      (0x02 if x & 0x40 != 0 else 0) |
      (0x04 if x & 0x20 != 0 else 0) |
      (0x08 if x & 0x10 != 0 else 0) |
      (0x10 if x & 0x08 != 0 else 0) |
      (0x20 if x & 0x04 != 0 else 0) |
      (0x40 if x & 0x02 != 0 else 0) |
      (0x80 if x & 0x01 != 0 else 0))
  fa[c[0] - 32] = newc

f = open('font.h', 'wt')
for c in fa:
  f.write('    {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X},\n' % (
    c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7],))
f.close()

"""
