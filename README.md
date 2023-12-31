## Connected_Plier


# Introduction
This project's goal is to centralise various works made by several teams of Sorbonne's RoMarin Project and Luning ZHU in order to create a connected plier that could, in the future, detect, follow and grab specific objects under the sea. 

# Functionalities 
The plier dispose of 3 different modes (automatic, semi-auto and manual) in order to control both horizontal movements and opening/closing movements while detecting a colored object. Indeed, at the end of the project, the plier was able to detect a green ball, position itself in the ball's axis, evaluate the distance and close itself, while controling the pinching force, when the ball was in the right grabbing distance.

 - In the first automatic mode, the pliers rotate hortizontaly until the ball is detected and then proceed to follow it and  
   grab it when the distance allows it.
 - The semi automatic mode allows for manual controls of the horizontal position but the grabbing part is automatic. 
 - The full manual mode allows manual control for both horizontal position and closing/opening of the pliers.

[![Plier](Media/image/Plier.gif)]

# Repo
- [Arduino code](Arduino_code) folder holds the arduino code for motors.
- [media](Media) folder contains pictures and videos about tests we made and robot structure.
- [Vision](Vision) Python code for detection modules.
- [Specs](Specs) Material used for the project



