import os
import glob
import subprocess

# Replace this with your render output directory
render_dir = "C:/Output/"
# Replace this with the path to your PTStitcherNG.exe or PTStitcherNG_cuda.exe path
stitcher_path = "C:/PTStitcher/PTStitcherNG.exe"

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
                print "Finished Converting: %s" % (tif_file)

