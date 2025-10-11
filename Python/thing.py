import time
import pyautogui
import keyboard
import sys

# Define the two strings to type
string1 = "0,0,0,0,0"
string2 = "180,180,180,180,180"

# Delay before starting (so you can switch to the target window)
print("Starting in 3 seconds...")
time.sleep(3)

count = 0
type180 = False

try:
    while True:
        if (not type180):
            pyautogui.typewrite(string1, interval=0.025)
        else:
            pyautogui.typewrite(string2, interval=0.025)
        
        pyautogui.press('enter')
            
    # Detect Ctrl+C using the keyboard library
        if keyboard.is_pressed('c'):
            print("\nC detected — stopping.")
            break

        count += 1
        if (count % 4 == 0):
            type180 = not type180

except KeyboardInterrupt:
    # This also catches the *real* Ctrl+C signal if pressed in terminal
    print("\nKeyboardInterrupt received — exiting.")
