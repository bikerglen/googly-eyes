#!/usr/bin/env python

import numpy as np
import cv2
import cv2.cv as cv
import serial
from video import create_capture
from common import clock, draw_str

help_message = '''
USAGE: facedetect.py [--cascade <cascade_fn>] [--nested-cascade <cascade_fn>] [<video_source>]
'''

def detect(img, cascade):
    rects = cascade.detectMultiScale(img, scaleFactor=1.3, minNeighbors=4, minSize=(30, 30), flags = cv.CV_HAAR_SCALE_IMAGE)
    if len(rects) == 0:
        return []
    rects[:,2:] += rects[:,:2]
    return rects

def draw_rects(img, rects, color):
    for x1, y1, x2, y2 in rects:
        cv2.rectangle(img, (x1, y1), (x2, y2), color, 2)

if __name__ == '__main__':
    import sys, getopt
    print help_message

    args, video_src = getopt.getopt(sys.argv[1:], '', ['cascade=', 'serial='])
    try: video_src = video_src[0]
    except: video_src = 0
    args = dict(args)
    cascade_fn = args.get('--cascade', "haarcascade_frontalface_alt.xml")
    serial_dev = args.get('--serial', "/dev/ttyUSB0")

    cascade = cv2.CascadeClassifier(cascade_fn)

    cam = create_capture(video_src, fallback='synth:bg=../cpp/lena.jpg:noise=0.05')

    cam.set(3,1280)
    cam.set(4,720)

    ser = serial.Serial (serial_dev, 19200)

    x = 0
    while True:
        ret, img = cam.read()
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        gray = cv2.equalizeHist(gray)

        t = clock()
        rects = detect(gray, cascade)
        vis = img.copy()
        draw_rects(vis, rects, (0, 255, 0))
	max_area = 0;
        mx1 = 0;
        my1 = 0;
        mx2 = 0;
        mx2 = 0;

        for x1, y1, x2, y2 in rects:
            area = abs ((x2 - x1) * (y2 - y1))
            if (area > max_area):
                max_area = area
                mx1 = x1
                mx2 = x2
                my1 = y1
                my2 = y2

        if (max_area != 0):
            cv2.rectangle(vis, (mx1, my1), (mx2, my2), (0, 0, 255), 2)
            x = (x1 + x2) / 2;
            x = -1 * (x - 640) * 800 / 640;
            ser.write ('mt {0:d} {0:d}\r'.format(x,x))
            # print x
            # print ('mt {0:d} {0:d}\r'.format(x,x))

        ser.flushInput ();
        dt = clock() - t
	# vis = np.fliplr (vis)
        vis = cv2.flip (vis, 1)
        draw_str(vis, (20, 20), 'time: %.1f ms' % (dt*1000))
        draw_str(vis, (20, 40), 'pos:  %4d' % (x))

        cv2.imshow('facedetect', vis)

        if 0xFF & cv2.waitKey(5) == 27:
            break
    cv2.destroyAllWindows()
    ser.close()
