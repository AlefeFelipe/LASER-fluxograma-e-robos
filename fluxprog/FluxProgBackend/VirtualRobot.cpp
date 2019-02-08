#include "VirtualRobot.hpp"

VirtualRobot::VirtualRobot()
{
    string serverIP = "127.0.0.1";
    int serverPort = 19999;
    clientID = simxStart((simxChar*)serverIP.c_str(),serverPort,true,true,2000,5);
    if (clientID != -1)
	{
        bool conected = true;
		cout << "Servidor conectado!" << std::endl;

        // inicializacao do robo
        if(simxGetObjectHandle(clientID,"bubbleRob",&robot, simx_opmode_oneshot_wait)==simx_return_ok)
        {
            cout << "conectado ao robot" <<endl;
        }
        else
        {
            conected = false;
        }

        //inicializacao dos motores
        if(simxGetObjectHandle(clientID, "bubbleRob_leftMotor", motors[0], simx_opmode_oneshot_wait) == simx_return_ok&&
            simxGetObjectHandle(clientID, "bubbleRob_rightMotor", motors[1], simx_opmode_oneshot_wait) == simx_return_ok&&)
        {
            cout << "conectado aos motores" << std::endl;
        }
        else
        {
            conected = false;
        }

        //inicializacao dos sensores ultrassom
        if(simxGetObjectHandle(clientID, "Left_ultrasonic", &ultrasonic_sensors[0], simx_opmode_oneshot_wait )==simx_return_ok&&
            simxGetObjectHandle(clientID, "LM_ultrasonic", &ultrasonic_sensors[1], simx_opmode_oneshot_wait )==simx_return_ok&&
            simxGetObjectHandle(clientID, "Middle_ultrasonic", &ultrasonic_sensors[2], simx_opmode_oneshot_wait )==simx_return_ok&&
            simxGetObjectHandle(clientID, "RM_ultrasonic", &ultrasonic_sensors[3], simx_opmode_oneshot_wait )==simx_return_ok&&
            simxGetObjectHandle(clientID, "Right_ultrasonic", &ultrasonic_sensors[4], simx_opmode_oneshot_wait )==simx_return_ok)
        {
                cout << "conectado aos sensores ultrassom" <<endl;
        }
        else
        {
            conected = false;
        }

        //inicializacao dos sensores de visao
        if(simxGetObjectHandle(clientID, "Left_Vision_sensor", &vision_sensors[0], simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "LM_Vision_sensor", &vision_sensors[1], simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "Middle_Vision_sensor", &vision_sensors[2], simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "RM_Vision_sensor", &vision_sensors[3], simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "Right_Vision_sensor", &vision_sensors[4], simx_opmode_oneshot_wait)==simx_return_ok)
        {
                cout << "conectado aos sensores de visao" <<endl;
        }
        else
        {
            conected = false;
        }

        if(conected)
        {
            simxGetObjectPosition(clientID, robot, -1, &robot_linear_position, simx_opmode_streaming);
            simxGetObjectOrientation(clientID, robot, -1, &robot_angle, simx_opmode_streaming);
            int i;
            for(i = 0; i < N_ULTRASONIC; i++)
            {
                simxReadProximitySensor(clientID, ultrasonic_sensors[i], &is_there_obstacle[i], &detected_objet[i], &detected_object_handle[i], &detected_surface[i], simx_opmode_streaming);
                ultrasonic_sensor_reading[i] = detected_objet[i][0];
            }
            for(i = 0; i < N_BLACK_TAPE_SENSOR; i++)
            {
                simxReadVisionSensor(clientID, vision_sensors[i], &is_there_color[i], &vision_sensor_data[i], &aux_vision_sensor[i], simx_opmode_streaming);
                black_type_sensor_reading[i] = vision_sensor_data[i][10] < MAX_INTE; //media da intensidade. No caso preto, sao todos 0
            }
        }
        else
        {
            ~VirtualRobot();
        }
	}
	else
    {
        ~VirtualRobot();
    }
}

VirtualRobot::~VirtualRobot()
{
    simxFinish(clientID);
}

void VirtualRobot::setCommand(int _command);
{
    command = _command;
    if(command)
    {
        sortCommand();
    }
}

void VirtualRobot::sortCommand()
{
    switch(command)
    {
        case 8:
            moveForward();
        break;
        case 6:
            turnRight();
        break;
        case 4:
            turnLeft();
        break;
        default:
            stop();
    }
}

void VirtualRobot::moveForward()
{
    int turning_tight, turning_left, first_x, first_y, dx, dy;
    if (is_there_obstacle[2]==0)
    {
        while(((black_type_sensor_reading[0]&&black_type_sensor_reading[2])||
                (black_type_sensor_reading[2]&&black_type_sensor_reading[4])) &&
                is_there_obstacle[2]==0)//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
        {
            motor_speed[0] = VEL_MOT;
            motor_speed[1] = VEL_MOT;
            simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            extApi_sleepMs(5);
            if(is_there_obstacle[2])
            {
                cout<<"objeto "<<detectedObjetHandle[2]<<" na posicao "<<detectedObjet[2][0]<<", "<<detectedObjet[2][1]<<", "<<detectedObjetMU[2][2]<<endl;
                cout<<"superficie em "<<detectedSurface[2][0]<<", "<<detectedSurface[2][1]<<", "<<detectedSurface[2][2]<<endl;
            }
        }
        while(!((black_type_sensor_reading[0]&&black_type_sensor_reading[2])||
                (black_type_sensor_reading[2]&&black_type_sensor_reading[4])) &&
                is_there_obstacle[2]==0)//anda ate os sensores captarem a linha ou obstaculo
        {
            motor_speed[0] = VEL_MOT;
            motor_speed[1] = VEL_MOT;
            simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            //if(!black_type_sensor_reading[2]&&(black_type_sensor_reading[3]||black_type_sensor_reading[4]))//robo esta desviando para a esquerda, gira direita
            if(black_type_sensor_reading[4])
            {
                motor_speed[0] = VEL_MOT;
                motor_speed[1] = VEL_MOT/4;
                simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
                updateVirtualData();
                turning_tight = 1;
                turning_left = 0;
            }
            else if(black_type_sensor_reading[3])
            {
                motor_speed[0] = VEL_MOT;
                motor_speed[1] = VEL_MOT/2;
                simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
                updateVirtualData();
                turning_tight = 1;
                turning_left = 0;
            }
            else if(black_type_sensor_reading[1])//robo ta desviando para a direita, gira esquerda
            {
                motor_speed[0] = VEL_MOT/2;
                motor_speed[1] = VEL_MOT;
                simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
                updateVirtualData();
                turning_tight = 0;
                turning_left = 1;
            }
            else if(black_type_sensor_reading[0])//robo ta desviando para a direita, gira esquerda
            {
                motor_speed[0] = VEL_MOT/4;
                motor_speed[1] = VEL_MOT;
                simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
                updateVirtualData();
                turning_tight = 0;
                turning_left = 1;
            }
            else if(!(black_type_sensor_reading[0]||black_type_sensor_reading[1]||
                black_type_sensor_reading[2]||black_type_sensor_reading[3]||black_type_sensor_reading[3]))
            {
                if(turning_tight)
                {
                    motor_speed[0] = VEL_MOT;
                    motor_speed[1] = VEL_MOT/4;
                    simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                    simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
                    updateVirtualData();
                }
                else if(turning_left)
                {
                    motor_speed[0] = VEL_MOT/4;
                    motor_speed[1] = VEL_MOT;
                    simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                    simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
                    updateVirtualData();
                }
            }
            if(is_there_obstacle[2])
            {
                cout<<"objeto "<<detectedObjetHandle[2]<<" na posicao "<<detectedObjet[2][0]<<", "<<detectedObjet[2][1]<<", "<<detectedObjetMU[2][2]<<endl;
                cout<<"superficie em "<<detectedSurface[2][0]<<", "<<detectedSurface[2][1]<<", "<<detectedSurface[2][2]<<endl;
            }
            extApi_sleepMs(5);
        }
        first_x = robot_linear_position[0];
        first_y = robot_linear_position[1];
        dx = 0;
        dy = 0;
        while((dx < DIS_RETO && dy <DIS_RETO) && is_there_obstacle[2]==0)//chegar na linha
        {
            motor_speed[0] = VEL_MOT;
            motor_speed[1] = VEL_MOT;
            simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            dx = abs(robot_linear_position[0]-first_x);
            dy = abs(robot_linear_position[1]-first_y);
            extApi_sleepMs(5);
            if(is_there_obstacle[2])
            {
                cout<<"objeto "<<detectedObjetHandle[2]<<" na posicao "<<detectedObjet[2][0]<<", "<<detectedObjet[2][1]<<", "<<detectedObjetMU[2][2]<<endl;
                cout<<"superficie em "<<detectedSurface[2][0]<<", "<<detectedSurface[2][1]<<", "<<detectedSurface[2][2]<<endl;
            }
        }
        command = 1;
    }
}

void VirtualRobot::turnRight()
{
    int first_ang, d_ang;
    first_ang = angle[2];
    while(d_ang < (M_PI/4-DIS_CURVA))//escapar do estado de verificacao dos sensores
    {
        motor_speed[0] = VEL_MOT/1.5;
        motor_speed[1] = -VEL_MOT/1.5;
        simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
        simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
        updateVirtualData();
        d_ang = abs(angle[2] - first_ang);
        //se houve salto da atan, ignora colocando o salto na posicao normal. Como abs, mas funciona melhor
        if(d_ang>(M_PI/2-D_ANG))
        {
            angle[2] = -angle[2];
            d_ang = abs(angle[2] - first_ang);
        }
        extApi_sleepMs(5);
    }
    while(black_type_sensor_reading[2])//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
    {
        motor_speed[0] = VEL_MOT/1.5;
        motor_speed[1] = -VEL_MOT/1.5;
        simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
        simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
        updateVirtualData();
        extApi_sleepMs(5);
    }
    while(!(black_type_sensor_reading[2]))//gira ate os sensores captarem a linha
    {
        motor_speed[0] = VEL_MOT/1.5;
        motor_speed[1] = -VEL_MOT/1.5;
        simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
        simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
        updateVirtualData();
        black_type_sensor_reading[2] = DataMVS[10]<MAX_INTE;
        black_type_sensor_reading[3] = DataRMVS[10]<MAX_INTE;
        extApi_sleepMs(5);
    }
    command = 1;
}

void VirtualRobot::turnLeft()
{
    int first_ang, d_ang;
    first_ang = angle[2];
    while(d_ang < (M_PI/4-DIS_CURVA))//escapar do estado de verificacao dos sensores
    {
        motor_speed[0] = -VEL_MOT/1.5;
        motor_speed[1] = VEL_MOT/1.5;
        simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
        simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
        updateVirtualData();
        d_ang = abs(angle[2] - first_ang);
        //se houve salto da atan, ignora colocando o salto na posicao normal. Como abs, mas funciona melhor
        if(d_ang>(M_PI/2-D_ANG))
        {
            angle[2] = -angle[2];
            d_ang = abs(angle[2] - first_ang);
        }
        extApi_sleepMs(5);
    }
    while((black_type_sensor_reading[2]))//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
    {
        motor_speed[0] = -VEL_MOT/1.5;
        motor_speed[1] = VEL_MOT/1.5;
        simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
        simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
        updateVirtualData();
        extApi_sleepMs(5);
    }
    while(!(black_type_sensor_reading[2]))//gira ate os sensores captarem a linha
    {
        motor_speed[0] = -VEL_MOT/1.5;
        motor_speed[1] = VEL_MOT/1.5;
        simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
        simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
        updateVirtualData();
        extApi_sleepMs(5);
    }
    command = 1;
}

void VirtualRobot::stop();
{
    motor_speed[0] = 0;
    motor_speed[1] = 0;
    simxSetJointTargetVelocity(clientID, motor[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
    simxSetJointTargetVelocity(clientID, motor[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
}

void VirtualRobot::updateSensorsReading()
{
    updateVirtualData();
}

void VirtualRobot::updateVirtualData()
{
    simxGetObjectPosition(clientID, robot, -1, &robot_linear_position, simx_opmode_buffer);
    simxGetObjectOrientation(clientID, robot, -1, &robot_angle, simx_opmode_buffer);
    int i;
    for(i = 0; i < N_ULTRASONIC; i++)
    {
        simxReadProximitySensor(clientID, ultrasonic_sensors[i], &is_there_obstacle[i], &detected_objet[i], &detected_object_handle[i], &detected_surface[i], simx_opmode_buffer);
        ultrasonic_sensor_reading[i] = detected_objet[i][0];
    }
    for(i = 0; i < N_BLACK_TAPE_SENSOR; i++)
    {
        simxReadVisionSensor(clientID, vision_sensors[i], &is_there_color[i], &vision_sensor_data[i], &aux_vision_sensor[i], simx_opmode_buffer);
        black_type_sensor_reading[i] = vision_sensor_data[i][10] < MAX_INTE; //media da intensidade. No caso preto, sao todos 0
    }
}
