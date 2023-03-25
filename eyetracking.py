import cv2 as cv
import mediapipe as mp
import time

from numpy import greater
import numpy as np
import utils
import os

# variables 
frame_counter =0

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
        (l_cx, l_cy), l_radius = cv.minEnclosingCircle(mesh_points[LEFT_IRIS])
        (r_cx, r_cy), r_radius = cv.minEnclosingCircle(mesh_points[RIGHT_IRIS])

        #get center of left/right iris in eye
        center_left = np.array([l_cx, l_cy], dtype = np.int32)
        center_right = np.array([r_cx, r_cy], dtype = np.int32)

        #draw circle around irises
        cv.circle(frame, center_left, int(l_radius), (255,0,255), 1, cv.LINE_AA)
        cv.circle(frame, center_right, int(r_radius), (255,0,0), 1, cv.LINE_AA)

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