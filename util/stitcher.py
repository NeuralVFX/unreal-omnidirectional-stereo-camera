import os
import glob
import subprocess
import argparse


parser = argparse.ArgumentParser()

parser.add_argument("cmd", help=argparse.SUPPRESS, nargs="*")
parser.add_argument('--stitcher', nargs='?', default="C:/PTStitcher/PTStitcherNG.exe", type=str)
parser.add_argument('--outdir', nargs='?', default="C:/Output/", type=str)

params = vars(parser.parse_args())

render_dir = params["outdir"]
stitcher_path =  params["stitcher"]

eyes = ['Right', 'Left']

for eye in eyes:
    txt_list = glob.glob('%s%s/*.txt' % (render_dir, eye))

    if txt_list:
        os.chdir('%s%s/' % (render_dir, eye))
        for txt in txt_list:
            txt_file = os.path.basename(txt)
            tif_file = txt_file.replace('txt', 'tif')
            command = "%s -f %s -o %s" % (stitcher_path, txt_file, tif_file)
            if subprocess.call(command) == 0:
                print ("Finished Converting: %s/%s" % (eye,tif_file))

