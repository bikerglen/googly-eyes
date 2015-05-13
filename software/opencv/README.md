fd2.py finds the biggest face on a webcam and relays the position of that face to an Arduino controlling the googly eyes. It is written in Python and requires numpy and opencv to run.

fd2.py is a modified version of opencv-2.4.9/samples/python2/facedetect.py. You will need to download the original opencv-2.4.9 distribution and copy a few additional files from it to get fd2.py to run.

To run fd2.py:

 -- create a new directory and cd to it. 
 
 -- copy fd2.py to the new directory.

 -- copy opencv-2.4.9/samples/python2/video.py to the new directory.

 -- copy opencv-2.4.9/samples/python2/common.py to the new directory.

 -- copy opencv-2.4.9/data/haarcascades/haarcascade_frontalface_alt.xml to the new directory.

Once that's done, execute the following from the command line:

python fd2.py --serial /dev/ttyUSB0

You may need to change /dev/ttyUSB0 to something different depending on which tty was mapped to the Arduino when the Arduino was connected. On my system, it was /dev/ttyACM0.
