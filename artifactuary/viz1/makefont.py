mode = 'font'
curchar = None

chars = []

lineno = 0

for line in open('c64.bdf', 'rt').readlines():
  line = line.strip('\n')
  #print 'processing "%s"' % (line,)
  lineno += 1
  if mode == 'font':
    if line.startswith('STARTCHAR '):
      kw, rest = line.split(' ', 1)
      charno = int(rest[1:])
      originalcharno = charno
      if charno >= 256:
        charno -= 256
        if charno < 32:
          charno += 96
      elif charno < 32:
        charno += 64
      curchar = (charno, [], originalcharno,)
      mode = 'char'
  elif mode == 'char':
    if line == 'BITMAP':
      mode = 'bitmap'
  elif mode == 'bitmap':
    if line == 'ENDCHAR':
      if len(curchar[1]) != 8:
        print 'char %d (line %d) wrong # lines in bitmap' % (curchar[0], lineno,)
      mode = 'font'
      chars.append(curchar)
      curchar = None
    else:
      curchar[1].append(int(line, 16))

chars.sort(lambda x, y: cmp(x[0], y[0]))

"""
for c in chars:
  print 'CHAR %d (%s) (was %d)' % (c[0], chr(c[0]) if c[0] >= 32 and c[0] < 256 else '?', c[2])
  for x in c[1]:
    print '%c%c%c%c%c%c%c%c' % (
      'X' if x & 0x80 != 0 else ' ',
      'X' if x & 0x40 != 0 else ' ',
      'X' if x & 0x20 != 0 else ' ',
      'X' if x & 0x10 != 0 else ' ',
      'X' if x & 0x08 != 0 else ' ',
      'X' if x & 0x04 != 0 else ' ',
      'X' if x & 0x02 != 0 else ' ',
      'X' if x & 0x01 != 0 else ' ',
      )
"""

f = open('c64.bitfont', 'wb')

def writeuint8(v):
  global f
  f.write(chr(v))

def writevlqhigh(v):
  global f
  if v >= 0x80:
    writevlqhigh(v >> 7)
  writeuint8(0x80 | (v & 0x7F))

def writevlq(v):
  global f
  if(v >= 0x80):
    writevlqhigh(v >> 7)
  writeuint8(v & 0x7F)

f.write('BTFT')
writevlq(8)
writevlq(len(chars))

for c in chars:
  writevlq(c[0])
  writevlq(8)
  for x in c[1]:
    writeuint8(
      (0x01 if x & 0x80 != 0 else 0) |
      (0x02 if x & 0x40 != 0 else 0) |
      (0x04 if x & 0x20 != 0 else 0) |
      (0x08 if x & 0x10 != 0 else 0) |
      (0x10 if x & 0x08 != 0 else 0) |
      (0x20 if x & 0x04 != 0 else 0) |
      (0x40 if x & 0x02 != 0 else 0) |
      (0x80 if x & 0x01 != 0 else 0))

f.close()

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

f = open('c64.h', 'wt')
for c in fa:
  f.write('    {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X},\n' % (
    c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7],))
f.close()

