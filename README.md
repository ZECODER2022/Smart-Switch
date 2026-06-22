# Smart Switch

This is a project ive been wanting to do for a while just haven't found a reason too until now. This project is a simple Govee light controller that uses the Govee Developer APi key to allow the user to turn on/off three different lights along with change the brightness. This is the type of thing that would be used for more casual lighting in common areas of a home like; a living room, kitchen, bathroom. It allows easy control over lights in simple ways!

<h2>Firmware:</h2>
In the Firmware at the top you connect it to Wifi along with your govee Api. For each of the three Lights you put in their: Mac Address, Along with their Model Number. (Image 1)

This allows them to be easily changed in the future with a small firmware change.

**Image 1:** <br>
<img width="274" height="253" alt="Screenshot 2026-06-20 at 10 33 51 PM" src="https://github.com/user-attachments/assets/4c3ccb67-82fa-44e4-aa21-d57118d03135" />

<h2>How it works</h2>
This device when turned on will connect to your WiFi then to Govee API, If for some reason it fails it will try again! When one of the button Zones are Pressed for the first time it will turn that device on, and when pressed again it will turn that device off. 

The Other 2 buttons on the device are the brightness buttons which when the Last button is pressed it will make your light you clicked last go to 30% brightness. When the 2nd to last button is pressed it will make the Light go to 100% Brightness

All the buttons: <br>
<img width="645" height="386" alt="Screenshot 2026-06-21 at 11 57 49 PM" src="https://github.com/user-attachments/assets/e57e79e1-6097-43ab-a4a7-2fc071afa958" />
