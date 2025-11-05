# MCU DLL FlowChart
  1. mcu2_initialization, mcu2_serial_connect
  2. 1번 과정 완료 후 mcu2_connect_check
  3-(1). 2번 과정에서 No일 경우, mcu2_re_connect
  3-(2). 2번 과정에서 Yes일 경우, Get, Set Function, Mode Setup
  4. 3번 과정 이후 Motion Status, 이후 계속해서 명령을 내릴 경우 다시 2번 과정으로 돌아간다.
  5. 4번 과정에서 End 할 경우, mcu2_terminate

# Function Summary
|   |   |
|:--|:--|
| int mcu2_ initialization(void) | DLL Class 생성 |
| int mcu2_terminate(void) | DLL 의 Serial Port, Thread, Class 해제 |
| int mcu2_serial_connect(int port) | Serial Port 설정 및 생성 |
| int mcu2_connect_check(void) | Serial Port 연결 상태 확인 |
| int mcu2_re_connect(void) | Serial Port 재 접속 |
| void mcu2_timeout_set(int interval, int wait_count) | 통신 대기시간과 횟수를 설정합니다 |
| int mcu2_get_l_variable(int address, int &data) | L 변수의 데이터를 읽음 |
| int mcu2_get_l_variables(int address, int cnt, int *data) | 여러 개의 L 변수의 데이터를 읽음 |
| int mcu2_set_l_variable(int address, int &data) | L 변수에 데이터를 씀 |
| int mcu2_get_p_variable(int address, int &data, int axis) | P 변수의 데이터를 읽음 |
| int mcu2_set_p_variable(int address, int &data, int axis) | P 변수에 데이터를 씀 |
| int mcu2_get_f_variable(int address, int &data) | F 변수의 데이터를 읽음 |
| int mcu2_set_f_variable(int address, int &data) | F 변수의 데이터를 씀 |
| int mcu2_get_d_variable(int address, int &data) | D 변수 의 데이터를 읽음 |
| int mcu2_set_d_variable(int address, int &data) | D 변수에 데이터를 씀 |
| int mcu2_get_t_point(int &data) | T 접점의 데이터를 읽음 |
| int mcu2_get_c_point(int &data) | C 접점의 데이터를 읽음 |
| int mcu2_get_m_point(int address, int &data) | M 접점의 데이터를 읽음 |
| int mcu2_get_m_points(int address, int cnt, int *data) | 여러 개의 M 접점의 데이터를 읽음 |
| int mcu2_set_m_point(int address, int access_method, int &data) | M 접점에 데이터를 씀 |
| int mcu2_get_x_point(int address, int &data) | X 접점의 데이터를 읽음 |
| int mcu2_get_y_point(int address, int &data) | Y 접점의 데이 터를 읽음 |
| int mcu2_set_y_point(int address, int &data) | Y 접점에 데이터를 씀 |
| int mcu2_set_auto_mode(void) | 제어기를 자동 모드로 설정합니다 |
| int mcu2_set_jog_mode(void) | 제어기를 수동 모드로 설정합니다 |
| int mcu2_set_org_mode(void) | 제어기를 원점복귀 모드로 설정합니다 |
| int mcu2_set_step_mode(void) | 제어기를 Step이동 모드로 설정합니다 |
| int mcu2_operation(void) | 자동, STEP 운전 기동 신호를 입력합니다 |
| int mcu2_reset(void) | Reset 신호를 입력합니다 |
| int mcu2_read_current_position(int &data, int axis) | 제어기에서 각 축의 위치를 읽어옵니다 |
| int mcu2_read_motor_position(int &data, int axis) | 제어기에서 각 모터의 위치를 읽어옵니다 |
| int mcu2_read_velocity(int &data) | 제어기에서 현재 속도를 읽어옵니다 |
| int mcu2_jog_origin_move(bool direct, bool start, int axis) | Jog, Origin 모드에서 이동 방향과 이동 축 설정 |
| int mcu2_origin_complete(int axis) | 원점 복귀 완료 신호를 읽어옵니다 |
| int mcu2_step_option(int *position, int velocity, bool move_sia, int axis) | 스텝 모드일 때 동작 신호를 입력합니다 |
| int mcu2_servo_control(int servo_control, int axis) | Servo를 ON/OFF합니다 |
| int mcu2_auto_override(int m_set, bool m_enable) | AUTO모드 OVERRIDE를 설정합니다 |
| int mcu2_jog_override(int m_set, bool m_enable) | STEP모드 OVERRIDE 를 설정합니다 |

# Parameter Set
|   |   |
|:--|:--|
| int mcu2_set_variable_data(void) | 제어기에 쓰여진 모든 variable을 Flash 메모리로 저장합니다 |
| int mcu2_read_plus_SWLimit _para(int &data, int axis) | + 방향 SW Limit 위치를 읽어옵니다 |
| int mcu2_write_plus_SWLimit_para(int &data, bool save_sel, int axis) | + 방향 SW Limit 위치를 저장합니다 |
| int mcu2_read_minus_SWLimit_para(int &data, int axis) | - 방향 SW Limit 위치를 읽어옵니다 |
| int mcu2_write_minus_SWLimit_para(int &data, bool save_sel, int axis) | - 방향 SW Limit 위치를 저장합니다 |
| int mcu2_read_maximum_velocity_para(int &data, int axis) | 최대 이송 속도를 읽어옵니다 |
| int mcu2_write_maximum_velocity_para(int &data, bool save_sel, int axis) | 최대 이송 속도를 저장합니다 |
| int mcu2_read_jog_velocity_para(int &data, int axis) | 수동 운전 속도를 읽어옵니다 |
| int mcu2_write_jog_velocity_para(int &data, bool save_sel, int axis) | 수동 운전 속도를 저장합니다 |
| int mcu2_read_homing_velocity1_para(int &data, int m_sel, int axis) | 1 축의 원점 복귀 속도를 읽어옵니다 |
| int mcu2_write_homing_velocity1_para(int &data, int m_sel, bool save_sel, int axis) | 1 축의 원점 복귀 속도를 저장합니다 |
| int mcu2_read_homing_velocity2_para(int &data, int m_sel, int axis) | 2축의 원점 복귀 속도를 읽어옵니다 |
| int mcu2_write_homing_velocity2_para(int &data, int m_sel, bool save_sel, int axis) | 2축의 원점 복귀 속도를 저장합니다 |
| int mcu2_read_motor_gain_para(int &data, int axis) | 모터 게인을 읽어옵니다 |
| int mcu2_write_motor_gain_para(int &data, bool save_sel, int axis) | 모터 게인을 저장합니다 |
| int mcu2_read_acc_dec_para(int &data, int axis) | 가감속을 읽어옵니다 |
| int mcu2_write_acc_dec_para(int &data, bool save_sel, int axis) | 가감속을 저장합니다 |
| int mcu2_read_decelerate_para(int &data, int axis) | 감속을 읽어옵니다 |
| int mcu2_write_decelerate_para(int &data, bool save_sel, int axis) | 감속을 저장합니다 |

# Return Code
|   |   |
|:--|:--|
| OK(0) | 정상적 완료 |
| NOT_INITIALIZED(1) | DLL 초기화가 되지 않았음 |
| NOT_FOUND(2) | 장치를 발견할 수 없음 |
| ILLEGAL_PARAMETER(8) | 범위 설정 오류 |
| CHECKSUM_ERROR(11) | Check sum 이 맞지 않습니다 |
| NACK_RETURN(12) | 통신 에러 |
| RETURN_COMMAND_ERROR(13) | 리턴 데이터 오류 |
| NO_SET(18) | 설정 실패 |
| N OT_EVENT(19) | 이벤트 생성 실패 |
| NOT_OPEN_PORT(20) | 포트 Open 실패 |
| FUNCTION_NOT_AVAILABLE(22) | 함수 접근이 불가능함 |
| NOT_THREAD(26) | 스레드 생성이 되지 않았음 |
| DISCONNECT(50) | 연결이 끊어짐 |

# initialization / terminate
## SYNTAX
  int mcu2_initialization(void)
  int mcu2_terminate(void)
## PROTOTYPE IN
  MCU2Common.h
## DESCRIPTION
  mcu2_ initialization(…) 함수는 Serial Port와 연결된 제어기를 운영하기 위해 초기화 하는 함수 입니다. DLL 의 기본 구조 Class 를 생성합니다. 따라서 이 함수 실행 후 제어기에 관련된 함수들을 정상적으로 호출할 수 있습니다.

  mcu2_serial_connect(…) 함수전에 반드시 선행 처리되어야만 Port설정과 Thread가 정상 작동합니다.

  mcu2_terminate(…) 함수는 DLL에 포함된 Serial Port, Thread, Class 등을 해제하기 위한 함수 입니다. 따라서 Memory Leaks를 방지하기 위하여 응용프로그램 종료시 반드시 이 함수를 호출하여야 합니다. 응용프로그램의 종료 함수에 응용프로그램의 Thread가 완전히 소멸된 것을 확인한 후 이 함수를 호출하는 것이 에러를 줄일 수 있습니다. 이 함수의 소멸 위치에 따라 DLL에서 에러를 발생할 수 있습니다.
## RETURN VALUES
  mcu2_initialization(…), mcu2_terminate(…)는 성공 여부를 반환합니다. 실패하면 NOT_INITIALIZED를 Return 합니다.
  OK(0), NOT_INITIALIZED(1)

# Serial_Connect / Reconnect
## SYNTAX
  int mcu2_serial_connect(int port)
  int mcu2_connect_check(void)
  int mcu2_re_connect(void)
  void mcu2_timeout_set(int interval, int wait_count)
## PROTOTYPE IN
  MCU2Common.h
## DESCRIPTION
  mcu2_serial_connect(…) 함수는 Serial Port 값을 정할 수 있습니다.
  Port 설정은 아래와 같습니다.
  -Serial Port
  | Value | Description |
  |:--|:--|
  | 1 | Com1 |
  | 2 | Com2 |
  | 3 | Com3 |
  | … | … |

  mcu2_connect_check(…) 함수는 제어기와 연결 상태를 확인하여 상태를 Return 합니다
  mcu2_re_connect(…) 함수는 제어기와 연결 상태가 끊어졌을 때, 다시 재 연결을 시켜줍니다
  mcu2 _timeout_set(…) 함수는 제어기와 통신 할 때 수신 대기시간을 설정하고 수신 실패 시 몇 번을 재시도 할지를 정할 수 있습니다. 첫 번째 interval 인자는 수신 대기시간을 ms 단위로 설정할 수 있고, wait_count 인자는 재시도 횟수를 설정합니다. 기본은 interval = 300, wait_count = 3 입니다.
## RETURN VALUES
  mcu2_serial_connect(…), mcu2_connect_check(…), mcu2 _re_connect(…)는 성공 여부를 반환합니다. 실패하면 오류 값을 Return 합니다.
  OK(0), NOT_FOUND(2), NOT_EVENT(19), NOT_OPEN_PORT(20), NOT_THREAD(26), DISCONNECT(50)
