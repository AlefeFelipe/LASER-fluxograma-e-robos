#include "VirtualRobot.hpp"

VirtualRobot::VirtualRobot(int *error)
{
    int i;
    for(i = 0; i < N_BLACK_TAPE_SENSOR; i++)
    {
        vision_sensor_data[i] = new float [VISION_SENSOR_COMPONENTS];
    }
    string serverIP = "127.0.0.1";
    int serverPort = 19999;
    client_id = 0;
    //*error = 1;
    client_id = simxStart((simxChar*)serverIP.c_str(),serverPort,true,true,2000,5);
    if (client_id != -1)
	{
        bool conected = true;
		//cout << "Servidor conectado!" << std::endl;

        // inicializacao do robo
        if(simxGetObjectHandle(client_id,"bubbleRob",&robot, simx_opmode_oneshot_wait)==simx_return_ok)
        {
            //cout << "conectado ao robot" <<endl;
        }
        else
        {
            conected = false;
        }

        //inicializacao dos motores
        if(simxGetObjectHandle(client_id, "bubbleRob_leftMotor", &motors[0], simx_opmode_oneshot_wait) == simx_return_ok&&
            simxGetObjectHandle(client_id, "bubbleRob_rightMotor", &motors[1], simx_opmode_oneshot_wait) == simx_return_ok)
        {
            //cout << "conectado aos motores" << std::endl;
        }
        else
        {
            conected = false;
        }

        //inicializacao dos sensores ultrassom
        if( simxGetObjectHandle(client_id, "LM_ultrasonic", &ultrasonic_sensors[0], simx_opmode_oneshot_wait )==simx_return_ok&&
            simxGetObjectHandle(client_id, "Middle_ultrasonic", &ultrasonic_sensors[1], simx_opmode_oneshot_wait )==simx_return_ok&&
            simxGetObjectHandle(client_id, "RM_ultrasonic", &ultrasonic_sensors[2], simx_opmode_oneshot_wait )==simx_return_ok)
        {
                //cout << "conectado aos sensores ultrassom" <<endl;
        }
        else
        {
            conected = false;
        }

        //inicializacao dos sensores de visao
        if(simxGetObjectHandle(client_id, "Left_Vision_sensor", &vision_sensors[0], simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(client_id, "LM_Vision_sensor", &vision_sensors[1], simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(client_id, "Middle_Vision_sensor", &vision_sensors[2], simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(client_id, "RM_Vision_sensor", &vision_sensors[3], simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(client_id, "Right_Vision_sensor", &vision_sensors[4], simx_opmode_oneshot_wait)==simx_return_ok)
        {
                //cout << "conectado aos sensores de visao" <<endl;
        }
        else
        {
            conected = false;
        }

        if(conected)
        {
            simxGetObjectPosition(client_id, robot, -1, robot_linear_position, simx_opmode_streaming);
            simxGetObjectOrientation(client_id, robot, -1, robot_angle, simx_opmode_streaming);
            int i;
            for(i = 0; i < N_ULTRASONIC; i++)
            {
                simxReadProximitySensor(client_id, ultrasonic_sensors[i], &is_there_obstacle[i], detected_objet[i], &detected_object_handle[i], detected_surface[i], simx_opmode_streaming);
                ultrasonic_sensor_reading[i] = detected_objet[i][1];
            }
            for(i = 0; i < N_BLACK_TAPE_SENSOR; i++)
            {
                simxReadVisionSensor(client_id, vision_sensors[i], &is_there_color[i], &vision_sensor_data[i], &aux_vision_sensor[i], simx_opmode_streaming);
                black_type_sensor_reading[i] = vision_sensor_data[i][10] < MAX_INTE; //media da intensidade. No caso preto, sao todos 0
            }
        }
        else
        {
            *error = 1;
        }
	}
    else
    {
        *error = 1;
    }

}

VirtualRobot::~VirtualRobot()
{
    simxFinish(client_id);
}

void VirtualRobot::setCommand(int _command)
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
        case MOVE_FORWARD:
            moveForward();
        break;
        case TURN_RIGHT:
            turnRight();
        break;
        case TURN_LEFT:
            turnLeft();
        break;
        default:
            stop();
    }
}

void VirtualRobot::moveForward()
{
    if(simxGetConnectionId(client_id) == -1)
    {

        command = CONNECT_ERROR;
    }
    else if(abstraction_level == LOW_ABSTRACTION)
    {
        motor_speed[0] = VEL_MOT;
        motor_speed[1] = VEL_MOT;
        simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
        simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
        updateVirtualData();
        extApi_sleepMs(5);
        command = 1;
        stop();
    }
    else
    {
        int turning_tight = 0, turning_left = 0;
        float first_x, first_y, dx, dy;
        //std::cout << (int)is_there_obstacle[1] <<std::endl;
        updateVirtualData();
        while(((black_type_sensor_reading[0]&&black_type_sensor_reading[2])||
                (black_type_sensor_reading[2]&&black_type_sensor_reading[4])) &&
                is_there_obstacle[1]==0 && simxGetConnectionId(client_id) != -1)//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
        {
            motor_speed[0] = VEL_MOT;
            motor_speed[1] = VEL_MOT;
            simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            extApi_sleepMs(5);
            if(is_there_obstacle[1])
            {
                //cout<<"objeto "<<detected_object_handle[2]<<" na posicao "<<detected_objet[2][0]<<", "<<detected_objet[2][1]<<", "<<detected_objet[2][2]<<endl;
                //cout<<"superficie em "<<detected_surface[2][0]<<", "<<detected_surface[2][1]<<", "<<detected_surface[2][2]<<endl;
            }
            if(simxGetConnectionId(client_id) == -1)
            {

                command = CONNECT_ERROR;
            }
        }
        while(!((black_type_sensor_reading[0]&&black_type_sensor_reading[2])||
                (black_type_sensor_reading[2]&&black_type_sensor_reading[4])) &&
                is_there_obstacle[1]==0 && simxGetConnectionId(client_id) != -1)//anda ate os sensores captarem a linha ou obstaculo
        {
            motor_speed[0] = VEL_MOT;
            motor_speed[1] = VEL_MOT;
            simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            //if(!black_type_sensor_reading[2]&&(black_type_sensor_reading[3]||black_type_sensor_reading[4]))//robo esta desviando para a esquerda, gira direita
            if(black_type_sensor_reading[4])
            {
                motor_speed[0] = VEL_MOT;
                motor_speed[1] = VEL_MOT/4;
                simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
                updateVirtualData();
                turning_tight = 1;
                turning_left = 0;
            }
            else if(black_type_sensor_reading[3])
            {
                motor_speed[0] = VEL_MOT;
                motor_speed[1] = VEL_MOT/2;
                simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
                updateVirtualData();
                turning_tight = 1;
                turning_left = 0;
            }
            else if(black_type_sensor_reading[1])//robo ta desviando para a direita, gira esquerda
            {
                motor_speed[0] = VEL_MOT/2;
                motor_speed[1] = VEL_MOT;
                simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
                updateVirtualData();
                turning_tight = 0;
                turning_left = 1;
            }
            else if(black_type_sensor_reading[0])//robo ta desviando para a direita, gira esquerda
            {
                motor_speed[0] = VEL_MOT/4;
                motor_speed[1] = VEL_MOT;
                simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
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
                    simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                    simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
                    updateVirtualData();
                }
                else if(turning_left)
                {
                    motor_speed[0] = VEL_MOT/4;
                    motor_speed[1] = VEL_MOT;
                    simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
                    simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
                    updateVirtualData();
                }
            }
            if(is_there_obstacle[1])
            {
                //cout<<"objeto "<<detected_object_handle[2]<<" na posicao "<<detected_objet[2][0]<<", "<<detected_objet[2][1]<<", "<<detected_objet[2][2]<<endl;
                //cout<<"superficie em "<<detected_surface[2][0]<<", "<<detected_surface[2][1]<<", "<<detected_surface[2][2]<<endl;
            }
            if(simxGetConnectionId(client_id) == -1)
            {

                command = CONNECT_ERROR;
            }
            extApi_sleepMs(5);
        }
        first_x = robot_linear_position[0];
        first_y = robot_linear_position[1];
        dx = 0;
        dy = 0;
        while((dx < DIS_RETO && dy <DIS_RETO) && is_there_obstacle[1]==0  && simxGetConnectionId(client_id) != -1)//chegar na linha
        {
            motor_speed[0] = VEL_MOT;
            motor_speed[1] = VEL_MOT;
            simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            dx = fabs(robot_linear_position[0]-first_x);
            dy = fabs(robot_linear_position[1]-first_y);
            extApi_sleepMs(5);
            if(is_there_obstacle[1])
            {
                //cout<<"objeto "<<detected_object_handle[2]<<" na posicao "<<detected_objet[2][0]<<", "<<detected_objet[2][1]<<", "<<detected_objet[2][2]<<endl;
                //cout<<"superficie em "<<detected_surface[2][0]<<", "<<detected_surface[2][1]<<", "<<detected_surface[2][2]<<endl;
            }
            if(simxGetConnectionId(client_id) == -1)
            {

                command = CONNECT_ERROR;
            }
        }
        stop();
        if(command >= 0)
        {
            command = 1;
        }
    }
}

void VirtualRobot::turnRight()
{
    if(simxGetConnectionId(client_id) == -1)
    {
        command = CONNECT_ERROR;
    }
    else if(abstraction_level == LOW_ABSTRACTION)
    {
        motor_speed[0] = VEL_MOT/1.5;
        motor_speed[1] = -VEL_MOT/1.5;
        simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
        simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
        updateVirtualData();
        extApi_sleepMs(5);
        command = 1;
        stop();
    }
    else
    {
        float first_ang = 0, d_ang = 0;
        updateVirtualData();
        first_ang = robot_angle[2];
        while(d_ang < (M_PI/4-DIS_CURVA)  && simxGetConnectionId(client_id) != -1)//escapar do estado de verificacao dos sensores
        {
            motor_speed[0] = VEL_MOT/1.5;
            motor_speed[1] = -VEL_MOT/1.5;
            simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            d_ang = fabs(robot_angle[2] - first_ang);
            //se houve salto da atan, ignora colocando o salto na posicao normal. Como fabs, mas funciona melhor
            if(d_ang>(M_PI/2-D_ANG))
            {
                robot_angle[2] = -robot_angle[2];
                d_ang = fabs(robot_angle[2] - first_ang);
            }
            if(simxGetConnectionId(client_id) == -1)
            {
                //
                command = CONNECT_ERROR;
            }
            extApi_sleepMs(5);
        }
        while(black_type_sensor_reading[2]  && simxGetConnectionId(client_id) != -1)//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
        {
            motor_speed[0] = VEL_MOT/1.5;
            motor_speed[1] = -VEL_MOT/1.5;
            simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            extApi_sleepMs(5);
            if(simxGetConnectionId(client_id) == -1)
            {

                command = CONNECT_ERROR;
            }
        }
        while(!(black_type_sensor_reading[2])  && simxGetConnectionId(client_id) != -1)//gira ate os sensores captarem a linha
        {
            motor_speed[0] = VEL_MOT/1.5;
            motor_speed[1] = -VEL_MOT/1.5;
            simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            extApi_sleepMs(5);
            if(simxGetConnectionId(client_id) == -1)
            {
                //
                command = CONNECT_ERROR;
            }
        }
        stop();
        if(command >= 0)
        {
            command = 1;
        }
    }
}

void VirtualRobot::turnLeft()
{
    if(simxGetConnectionId(client_id) == -1)
    {
        command = CONNECT_ERROR;
    }
    else if(abstraction_level == LOW_ABSTRACTION)
    {
        motor_speed[0] = -VEL_MOT/1.5;
        motor_speed[1] = VEL_MOT/1.5;
        simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
        simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
        updateVirtualData();
        extApi_sleepMs(5);
        command = 1;
        stop();
    }
    else
    {
        float first_ang = 0, d_ang = 0;
        updateVirtualData();
        first_ang = robot_angle[2];
        while(d_ang < (M_PI/4-DIS_CURVA)  && simxGetConnectionId(client_id) != -1)//escapar do estado de verificacao dos sensores
        {
            motor_speed[0] = -VEL_MOT/1.5;
            motor_speed[1] = VEL_MOT/1.5;
            simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            d_ang = fabs(robot_angle[2] - first_ang);
            //se houve salto da atan, ignora colocando o salto na posicao normal. Como fabs, mas funciona melhor
            if(d_ang>(M_PI/2-D_ANG))
            {
                robot_angle[2] = -robot_angle[2];
                d_ang = fabs(robot_angle[2] - first_ang);
            }
            if(simxGetConnectionId(client_id) == -1)
            {
                command = CONNECT_ERROR;
            }
            extApi_sleepMs(5);
        }
        while((black_type_sensor_reading[2])  && simxGetConnectionId(client_id) != -1)//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
        {
            motor_speed[0] = -VEL_MOT/1.5;
            motor_speed[1] = VEL_MOT/1.5;
            simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            if(simxGetConnectionId(client_id) == -1)
            {
                command = CONNECT_ERROR;
            }
            extApi_sleepMs(5);
        }
        while(!(black_type_sensor_reading[2])  && simxGetConnectionId(client_id) != -1)//gira ate os sensores captarem a linha
        {
            motor_speed[0] = -VEL_MOT/1.5;
            motor_speed[1] = VEL_MOT/1.5;
            simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
            simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
            updateVirtualData();
            if(simxGetConnectionId(client_id) == -1)
            {
                command = CONNECT_ERROR;
            }
            extApi_sleepMs(5);
        }
        stop();
        if(command >= 0)
        {
            command = 1;
        }
    }
}

void VirtualRobot::stop()
{
    if(simxGetConnectionId(client_id) == -1)
    {
        command = CONNECT_ERROR;
    }
    else
    {
        motor_speed[0] = 0;
        motor_speed[1] = 0;
        simxSetJointTargetVelocity(client_id, motors[0], (simxFloat) motor_speed[0], simx_opmode_streaming);
        simxSetJointTargetVelocity(client_id, motors[1], (simxFloat) motor_speed[1], simx_opmode_streaming);
    }
}

void VirtualRobot::updateSensorsReading()
{
    updateVirtualData();
}

void VirtualRobot::updateVirtualData()
{
    if(simxGetConnectionId(client_id) == -1)
    {
        command = CONNECT_ERROR;
    }
    else
    {
        simxGetObjectPosition(client_id, robot, -1, robot_linear_position, simx_opmode_buffer);
        simxGetObjectOrientation(client_id, robot, -1, robot_angle, simx_opmode_buffer);
        int i;
        for(i = 0; i < N_ULTRASONIC; i++)
        {
            simxReadProximitySensor(client_id, ultrasonic_sensors[i], &is_there_obstacle[i], detected_objet[i], &detected_object_handle[i], detected_surface[i], simx_opmode_buffer);
            //ultrasonic_sensor_reading[i] = detected_objet[i][2] * 100;
            ultrasonic_sensor_reading[i] = is_there_obstacle[i] * 20;
            if(abstraction_level > HIGH_ABSTRACTION)
            {
                if(is_there_obstacle[i] && (detected_objet[i][2]*20 < 1))
                {
                    std::cout << "oh nao, valor "<<detected_objet[i][2]*20<<std::endl;
                    command = COLISION;
                }
                else
                {
                    is_there_obstacle[i] = 0;
                }
            }
        }
        for(i = 0; i < N_BLACK_TAPE_SENSOR; i++)
        {
            simxReadVisionSensor(client_id, vision_sensors[i], &is_there_color[i], &vision_sensor_data[i], &aux_vision_sensor[i], simx_opmode_buffer);
            black_type_sensor_reading[i] = vision_sensor_data[i][10] < MAX_INTE; //media da intensidade. No caso preto, sao todos 0
        }
    }
}
