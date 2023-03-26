import cv2 as cv
import mediapipe as mp
import time

from numpy import greater
import numpy as np
import utils
import os
import socket

HOST = "172.20.10.9"  # Run ipconfig on this computer and grab ipv4 address
PORT = 80  # The port used by the server (can be changed to anything not reserved)

# initialize tcp connection
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    #Determine direction to move wheelchair based on location of iris relative to eye 
    # iris: tuple(x:int, y:int)
    # eye: tuple(x:int, y:int)
    # nFac: tuple(x:int, y:int) = normalization factor

    def direction(eye, iris, nFac):
        # global samples
        # global avgX
        # global avgY
        
        # samples += 1
        #Ensure no dividing by 0
        if (nFac[0] == 0):
            nFac = (1, nFac[1])
        if (nFac[1] == 0):
            nFac = (nFac[0], 1)

        # Determine iris location based on coordinate plane created by eye
        irisX = ((iris[0] - eye[0])+0.5)/nFac[0] # Normalize across eyeX
        irisY = ((iris[1] - eye[1])+0.5)/nFac[1] # Normalize across eyeY

        if (abs(irisX) > abs(irisY*1.2)):# We want to use the direction that the iris is furthest in
            if (irisX >= 0.5): #Iris is right
                return "RIGHT"
            elif(irisX <= -0.4): #Iris is left
                return "LEFT"
        else:
            if (irisY >= -0.152): #Iris is down
                return "DOWN"
            elif (irisY <= -0.4): #Iris is up
                return "UP"
        return "STRAIGHT"
        
    # variables 
    frame_counter = 0

    # constants 
    FONTS =cv.FONT_HERSHEY_COMPLEX
    LEFT_EYE = [362, 382, 381, 380, 374, 373, 390, 249, 263, 466, 388, 387, 386, 385, 384, 398]

    #right eyes indices 
    RIGHT_EYE=[33, 7, 163, 144, 145, 153, 154, 155, 133, 173, 157, 158, 159, 160, 161, 246]
    LEFT_IRIS=[474, 475, 476, 477]
    RIGHT_IRIS=[469, 470, 471, 472]

    map_face_mesh = mp.solutions.face_mesh

    # camera object 
    camera = cv.VideoCapture(0)

    with map_face_mesh.FaceMesh(
        min_detection_confidence =0.5, 
        min_tracking_confidence=0.5,
        max_num_faces=1,
        refine_landmarks = True
    ) as face_mesh:
        # starting time here 
        start_time = time.time()

        while True:
            frame_counter +=1 # frame counter
            ret, frame = camera.read() # getting frame from camera
            frame = cv.flip(frame,1) #flip frame

            rgb_frame = cv.cvtColor(frame, cv.COLOR_RGB2BGR)
            img_h, img_w = frame.shape[:2] 
            results  = face_mesh.process(rgb_frame)
            if results.multi_face_landmarks:
                #get mesh points from frame data
                mesh_points = np.array([np.multiply([p.x,p.y],[img_w, img_h]).astype(int) for p in results.multi_face_landmarks[0].landmark])

                #get coordinates of eye-data
                (l_iris_cx, l_iris_cy), l_iris_radius = cv.minEnclosingCircle(mesh_points[LEFT_IRIS])
                (r_iris_cx, r_iris_cy), r_iris_radius = cv.minEnclosingCircle(mesh_points[RIGHT_IRIS])
                (l_eye_cx, l_eye_cy), l_eye_radius = cv.minEnclosingCircle(mesh_points[LEFT_EYE])
                (r_eye_cx, r_eye_cy), r_eye_radius = cv.minEnclosingCircle(mesh_points[RIGHT_EYE])

                #get center of left/right iris in eye
                iris_center_left = np.array([l_iris_cx, l_iris_cy], dtype = np.int32)
                iris_center_right = np.array([r_iris_cx, r_iris_cy], dtype = np.int32)
                
                #get center of eye
                eye_center_left = np.array([l_eye_cx, l_eye_cy], dtype = np.int32)
                eye_center_right = np.array([r_eye_cx, r_eye_cy], dtype = np.int32)

                #draw circle around irises
                cv.circle(frame, iris_center_left, int(l_iris_radius), (255,0,255), 1, cv.LINE_AA)
                cv.circle(frame, iris_center_right, int(r_iris_radius), (255,0,0), 1, cv.LINE_AA)

                #determine direction of eye
                lDir = direction((l_eye_cx, l_eye_cy),(l_iris_cx, l_iris_cy), (l_eye_radius/2, r_eye_radius/2)) #left
                rDir = direction((r_eye_cx, r_eye_cy),(r_iris_cx, r_iris_cy), (l_eye_radius/2, r_eye_radius/2)) #right
                mDir = None #combine
                #Take precedene of direction that is not straight
                if(lDir == "STRAIGHT" and rDir == "STRAIGHT"):
                    mDir = "NONE"
                elif (lDir == "STRAIGHT"):
                    mDir = rDir
                elif (rDir == "STRAIGHT"):
                    mDir = lDir 
                
                #Take precedence of left or right before up/down
                elif (lDir == "RIGHT" or rDir == "RIGHT"): 
                    mDir = "RIGHT"
                elif (lDir == "LEFT" or rDir == "LEFT"): 
                    mDir = "LEFT"
                else:
                    mDir = rDir

                # Encode our eye direction into a bitstring:
                # Bin(0) => up (break)
                # Bin(1) => down (straight)
                # Bin(2) => left (left)
                # Bin(3) right (right)
                # All digits 0 if no direction ("NONE" output)
                bitstring = "00\n"
                if mDir == "UP":
                    bitstring = "00\n"
                elif mDir == "DOWN": 
                    bitstring = "01\n"
                elif mDir == "LEFT":
                    bitstring = "10\n"
                elif mDir == "RIGHT":
                    bitstring = "11\n"

                s.sendall(bitstring.encode()) #Send eye-direction data over TCP stream

                # calculating frame per seconds FPS
                end_time = time.time()-start_time
                fps = frame_counter/end_time

                # writing image for thumbnail drawing shape
                # cv.imwrite(f'img/frame_{frame_counter}.png', frame)
                cv.imshow('frame', frame)
                key = cv.waitKey(1)
                if key==ord('q') or key ==ord('Q'):
                    break
    cv.destroyAllWindows()
    camera.release()