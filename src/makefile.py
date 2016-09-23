from os.path import dirname, expanduser, join
from subprocess import call
from sys import argv, platform

if platform == "win32":
    arduino_folder = "C:\Program Files (x86)\Arduino\\"
    arduino_builder_folder = expanduser("~\\go_projects\\arduino-builder")
else:
    arduino_folder = expanduser("~/arduino-1.6.7/")
    arduino_builder_folder = expanduser("~/go_projects/arduino-builder")

teensyLC_build_vars = ["teensyLC.build.fcpu=48000000",
                       "teensyLC.build.flags.optimize=-Os",
                       "teensyLC.build.flags.ldspecs=--specs=nano.specs",
                       "teensyLC.build.keylayout=US_ENGLISH",
                       "teensyLC.build.usbtype=USB_SERIAL"]


def format_folder(folder, path):
    return "\"" + join(folder, path).replace("\\", "\\\\") + "\""


def format_arduino_folder(path):
    return format_folder(arduino_folder, path)


def check_boards():
    boards = join(arduino_folder, "hardware/teensy/avr/boards.txt")
    fh = open(boards, "r")
    lines = fh.readlines()
    orig_num = len(lines)
    fh.close()
    for line in teensyLC_build_vars:
        found_line = False
        for inline in lines:
            if inline.find(line) >= 0:
                found_line = True
        if not found_line:
            lines.append(line)
    new_num = len(lines)
    if new_num > orig_num:
        fh = open(boards, "w")
        fh.writelines(lines)
        fh.close()


def compile_upload(project_name="mitutoyo_interface"):
    # first, check the boards file for the build variables
    curr_dir = dirname(__file__)

    check_boards()
    command = [join(arduino_builder_folder, 'arduino-builder'),
               '-fqbn', 'teensy:avr:teensyLC',
               '-hardware', format_arduino_folder('hardware'),
               '-tools', format_arduino_folder('hardware/tools'),
               '-tools', format_arduino_folder('tools-builder'),
               '-libraries',
               format_arduino_folder('hardware/teensy/avr/libraries'),
               '-libraries', format_folder(join(curr_dir), project_name),
               format_folder(join(curr_dir, project_name), 'main.ino')]
    print(' '.join(command))
    call(command)

if __name__ == "__main__":
    compile_upload(project_name="mitutoyo_interface")

