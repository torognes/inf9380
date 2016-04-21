from pylab import *

t = arange(0.0, 2.0, 0.01)
s = sin(2*pi*t)
plot(t, s, linewidth=1.0)

xlabel('time (s)')
ylabel('voltage (mV)')
title('About as simple as it gets, folks')
grid(True)
show()
annotate('local max', xy=(1.25, 1.0), xytext=(1.4, 1.2), arrowprops={'shrink':0.05})
axis([0, 2, -1.0, 2])
gca().texts.pop()
