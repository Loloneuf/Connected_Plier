import numpy as np
import cv2
import time
import serial

###INITIALISATION
Valmotor=0
mask = np.array([[], []])
circles = np.array([[[]]])
cap = cv2.VideoCapture(0)
arduino=serial.Serial('com3',9600,timeout=0.1)
time.sleep(1)

xmin_fin, xmax_fin, ymin_fin, ymax_max = 0, 0, 0, 0
diametre = 4 #diameter of the object to be detected
alpha = 1.2217 #vision angle of the camera in radians
beta = alpha * 0.75
coord = [[], [], [], []]#[[xmin1, ..., xmin5], ..., [ymax1, ..., ymax5]] <=> [ xmin, xmax, ymin, ymax]

lower_green = np.array([65, 50, 50]) #minimum values of detection for our objects colour
upper_green = np.array([90, 255, 255]) #max values

# these values can be modified to adapt the system to the ambiant luminosity
width = int(cap.get(3)) 
height = int(cap.get(4)) 
xcenter = int(width/2) 
ycenter = int(height/2) 
print(xcenter)
while True:
    ymax, ymin = 0, mask.shape[0] 
    xmax, xmin = 0, mask.shape[1]
    
    ret, frame = cap.read() #frame = original video to be used for masking
    
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)# Rgb to gray
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV) #Rgb to hsv
    mask = cv2.inRange(hsv, lower_green, upper_green) # mask creation for green detection
    mask2 = np.zeros(frame.shape[:2], dtype="uint8") # mask for circle detection
    result = cv2.bitwise_and(frame, frame, mask=mask) # initial video but only with the color of interest 
    
    #HoughCircles filter to detect contours. 
    #param2 is the treshold to detect circles
    #minRadius min radius for circile detection in pixels
    circles = cv2.HoughCircles(gray,cv2.HOUGH_GRADIENT, 2, 60, param1=150,param2=80,minRadius=5,maxRadius=90)
    #now we need to check if we detected at least one circle
    if type(circles) == np.ndarray and circles.size != 0:
        circles = np.uint16(np.around(circles))#circles is a 2d matrix and each lines are [xcoorc, ycoord, radius]
        for i in circles[0,:]:
            #add a filled circle in our mask 2
            cv2.circle(mask2,(i[0],i[1]),i[2], 255, -1)
        result = cv2.bitwise_and(result, result, mask=mask2)#add the mask to our resulting video
        
    ymax, ymin = 0, result.shape[0]
    xmax, xmin = 0, result.shape[1]

    
    # np.where() is way faster than searching pixel by pixel (30 times faster)
    coord_result = np.where(result != 0) #coord result contains our object's pixels coordinates
    if coord_result[0].size > 0: #check if the colour is detected and send an error otehrwise
        ymin = coord_result[0][0]; ymax = coord_result[0][-1]
        coord_result[1].sort() #sorting x coordinates
        xmin = coord_result[1][0]; xmax = coord_result[1][-1]
        coord[0].append(xmin); coord[1].append(xmax); coord[2].append(ymin); coord[3].append(ymax)

    # compute the object position by taking the median over 31 itterations
    # send coordinates every 1 second.
    if len(coord[0]) != 0 and len(coord[0]) % 2 == 0:
        mediane = int((len(coord[0]) - 1)/ 2)

        #sorting so that the median value is the center of our vectors
        coord[0].sort(); coord[1].sort(); coord[2].sort(); coord[3].sort()
        xmin_fin, xmax_fin = coord[0][mediane], coord[1][mediane]
        ymin_fin, ymax_fin = coord[2][mediane], coord[3][mediane]
        coord = [[], [], [], []]
        
        #this rectangle should be the center of our object and we draw a cross for more accuracy
        cv2.rectangle(result, (xmin_fin, ymin_fin), (xmax_fin, ymax_fin), (255, 255, 255), 3)
        xmean, ymean = int((xmax_fin + xmin_fin)/2), int((ymax_fin + ymin_fin)/2) #compute the object's center coordinates
        cv2.line(result, (xmean , ymean - 10), (xmean, ymean + 10) , (0, 255, 0), 2)
        cv2.line(result, (xmean - 10, ymean), (xmean + 10, ymean) , (0, 255, 0), 2)
        #cv2.rectangle(result, (xcenter -80, ycenter - 80), (xcenter + 80, ycenter + 80) , (0, 255, 0), 2)
        #we use simple trigonometry to compute the distance
        distance = diametre/(2 * np.sin(np.arctan(np.tan(alpha/2)*(xmax - xmin)/mask.shape[1])))
        if distance > 0 and distance < 100:
            print("the object is at", distance, "cm")
            
    
        if xmean > xcenter +100 : # conditions on the center's position (+100 /-100 defines a 100 pixels line where we will consider our object as centered)
            Valmotor = 1 # the object is on the right
        elif xmean < xcenter -100 
            Valmotor = -1  #the object is on the left
        else :
            Valmotor = 0 # the object is centered
            
        if 10.2<distance<10.8 and Valmotor == 0: #conditions on distance between the object and the plier 
            Dist = 1 # the object is at the right distance to be grabbed
        else:
            Dist = 0
            #rom.hauteur(ymean, beta, distance)
            #rom.angle(xmean, alpha)
        string='X{0:d}Y{1:d}'.format(Valmotor,Dist)
        print(string)
        arduino.write(string.encode('utf-8'))

    
    #cv2.imshow('original video', frame) 
    cv2.imshow('Results', result) 
    #cv2.imshow("Colors_mask cherchée", mask)
    #cv2.imshow("Circles_mask", mask2)
    
    if cv2.waitKey(1) == ord('q'): #press q to stop the program
        break
    
cap.release() #end of the records
cv2.destroyAllWindows() #windows closure
