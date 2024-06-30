"""
   0------------------> x (cols) Image Frame
   |
   |        c    Camera frame
   |         o---> x
   |         |
   |         V y
   |
   V y (rows)

"""
import numpy as np
import cv2
import serial
import time
#해상도크기지정 
SCREEN_WIDTH = 1920		#960
SCREEN_HEIGHT = 1080		#540
#x,y 픽셀 기준값 
X_V_A=50
Y_V_A=100
#시리얼포트 지정, 통신속도 지정 
port = 'COM7'
baud = 9600
ser = serial.Serial(port, baud, timeout=0.1)
        
def nothing(x):
    pass

def main():

    global x_pos
    global y_pos
    
    capture = cv2.VideoCapture(1)	#내장캠우선0, 추가시1 노트북은1
    cv2.namedWindow("Trackbars",cv2.WINDOW_NORMAL)	#트랙바 창만들기, 크기 조절가능  
    
    cv2.createTrackbar("L - H", "Trackbars", 99, 255, nothing)		#85
    cv2.createTrackbar("L - S", "Trackbars", 120, 255, nothing)		#120
    cv2.createTrackbar("L - V", "Trackbars", 120, 255, nothing)		#70

    cv2.createTrackbar("U - H", "Trackbars", 120, 255, nothing)
    cv2.createTrackbar("U - S", "Trackbars", 255, 255, nothing)
    cv2.createTrackbar("U - V", "Trackbars", 255, 255, nothing)

    while(True):

        ret, frame = capture.read()
        #frame = cv2.resize(frame, (0,0), fx=0.5, fy=0.5)
        frame = cv2.resize(frame, (1920,1080),cv2.INTER_LANCZOS4)

        l_h = cv2.getTrackbarPos("L - H", "Trackbars")
        l_s = cv2.getTrackbarPos("L - S", "Trackbars")
        l_v = cv2.getTrackbarPos("L - V", "Trackbars")
        u_h = cv2.getTrackbarPos("U - H", "Trackbars")
        u_s = cv2.getTrackbarPos("U - S", "Trackbars")
        u_v = cv2.getTrackbarPos("U - V", "Trackbars")
        #HSV 색공간표현
        lower_blue = np.array([l_h, l_s, l_v])
        upper_blue = np.array([u_h, u_s, u_v])
        # 컬러 영상을 HSV 색공간으로 변환 H,S 색상 V밝기
        hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # HSV색공간에서 특정 색상 영역 추출 
        mask = cv2.inRange(hsv_frame, lower_blue, upper_blue)
        # mask 색공간의 외과선 검출         
        mask_contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
             
        # 윤곽선 정보를 이용하여 박스생성 및 좌표 추출 
        if len(mask_contours) != 0 :
            for mask_contour in mask_contours:
                if cv2.contourArea(mask_contour) > 2000:
                    #면적이 2000 넘으면 실행 
                    x, y, w, h = cv2.boundingRect(mask_contour)		#최소크기의 사각형 반환
                    cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2) #drawing rectangle
                    m=w//2
                    k=h//2
                    x_pos=960-(x+m)
                    y_pos=540-(y+k)
                    cv2.line(frame,(960,0),(960,1080),(0,255,0), 2)
                    cv2.line(frame,(0,540),(1920,540),(0,255,0), 2)
                    cv2.line(frame, (960,540), (x+m, y+k), (0,255,0),2,cv2.LINE_AA)
                    #print('X={} Y={} width={} height={} m={} C={}'.format( x, y, w, h, m, x_pos))
                
                    font=cv2.FONT_HERSHEY_SIMPLEX
        
                    X_axis = 'X_axis:{}  '.format(x_pos)
                    Y_axis = 'Y_axis:{}'.format(y_pos)

                    text_sum=X_axis+Y_axis
                    
                    cendR="R"
                    cendL="L"
                    cendC="S"
                    cendU="U"
                    cendD="D"
                    cendE="E"

                    cv2.rectangle(frame, (0, 990), (800, 1080), color=(0, 255, 255), thickness=-1)
                    frame = cv2.putText(frame, text_sum,(20, 1050),font, 2, (0, 0, 255), 3)

        
                    if x_pos>X_V_A:
                        ser.write(cendL.encode())
                    elif x_pos<-X_V_A:
                        ser.write(cendR.encode())         
                    else:
                        ser.write(cendE.encode())
                              
                    if y_pos>Y_V_A:
                        ser.write(cendU.encode())
                    elif y_pos<-Y_V_A:
                        ser.write(cendD.encode())         
                    else:
                        ser.write(cendE.encode())

                    if -X_V_A <= x_pos and X_V_A >= x_pos :
                        if  -Y_V_A<= y_pos and X_V_A >= y_pos :
                            ser.write(cendC.encode())
                            cv2.circle(frame, (960, 540), 15,(0, 0, 255), -1)
                            cv2.circle(frame, (960, 540), 30,(0, 0, 255), 2)
                        
        cv2.imshow("original image",frame)
        cv2.imshow("mask image",mask)
        
        if cv2.waitKey(1) == ord('q'):
            break
        
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
    ser.close()