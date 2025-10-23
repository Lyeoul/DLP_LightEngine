# DLP 프로젝터 제어 프로그램

DLP 프로젝터를 I2C 통신으로 제어하는 C 언어 기반 프로그램입니다. Cypress USB Serial 라이브러리를 사용하여 USB-to-I2C 브리지를 통해 프로젝터와 통신합니다.

## 목차
- [시스템 요구사항](#시스템-요구사항)
- [컴파일 및 실행](#컴파일-및-실행)
- [기능 설명](#기능-설명)
- [사용 방법](#사용-방법)
- [기술 사양](#기술-사양)
- [문제 해결](#문제-해결)

---

## 시스템 요구사항

### 하드웨어
- Cypress USB Serial 장치 (USB-to-I2C 브리지)
- DLP 프로젝터 (I2C 인터페이스 지원)
- I2C 연결: DLP 프로젝터와 USB Serial 장치 간

### 소프트웨어
- Windows OS (Windows 7 이상)
- MinGW 또는 Visual Studio (컴파일용)
- Cypress USB Serial 라이브러리 (`cyusbserial.dll`, `cyusbserial.lib`)

### I2C 주소
- **DLP 프로젝터**: 0x1B (Write: 0x36, Read: 0x37)
- **광센서**: 0x39 (Read: 0x73)

---

## 컴파일 및 실행

### 컴파일

#### MinGW 사용
```bash
gcc -o CyTest.exe CyTest.c -L. -lcyusbserial
```

#### Visual Studio 사용
```bash
cl CyTest.c cyusbserial.lib
```

### 실행
```bash
./CyTest.exe
```

**중요**: `cyusbserial.dll` 파일이 실행 파일과 같은 디렉토리에 있어야 합니다.

---

## 기능 설명

### 메인 메뉴

프로그램 실행 시 다음과 같은 메뉴가 표시됩니다:

```
========================================
   DLP Projector Control Menu
========================================
1.  Set LED Current
2.  Enable LED
3.  Disable LED
4.  Read LED Temperature
5.  Set Image Flip (No Flip)
6.  Set Image Flip (Horizontal)
7.  Set Image Flip (Vertical)
8.  Set Image Flip (Both)
9.  Read Light Sensor Value
10. Select HDMI Input
11. Select Ramp Test Pattern
12. Select Checker Test Pattern
0.  Exit
========================================
```

---

## 사용 방법

### 1. LED 전류 설정 (Set LED Current)

**메뉴 옵션**: `1`

LED의 구동 전류를 설정합니다.

#### 사용 방법:
1. 메뉴에서 `1` 입력
2. 원하는 전류 값을 mA 단위로 입력 (예: `5000`)

#### 전류 범위:
- **최소**: 5.86 mA
- **최대**: 6000 mA
- **권장**: 3000 ~ 5000 mA

#### 예시:
```
Select option: 1
Enter LED current in mA (5.86 to 6000): 5000
Setting LED current to 5000.00 mA (bit value: 0x0354)
I2C Write Success: Command 0x54, 7 bytes written
```

#### 계산 공식:
```
Bit Value = (Current_mA / 5.8593) - 1
Current_mA = (Bit Value + 1) × 5.8593
```

#### I2C 명령:
- **Command**: 0x54
- **Data[0]**: 0x00 (Reserved)
- **Data[1]**: 0x00 (Reserved)
- **Data[2]**: Bit Value (LSB)
- **Data[3]**: Bit Value (MSB)
- **Data[4]**: 0x00 (Reserved)
- **Data[5]**: 0x00 (Reserved)

---

### 2. LED 활성화 (Enable LED)

**메뉴 옵션**: `2`

LED를 켭니다.

#### 사용 방법:
1. 메뉴에서 `2` 입력
2. LED가 자동으로 켜집니다

#### 예시:
```
Select option: 2
Enabling LED...
I2C Write Success: Command 0x52, 2 bytes written
```

#### 주의사항:
- **비디오 신호가 없으면 LED가 자동으로 꺼집니다**
- LED를 켜기 전에 반드시 HDMI 입력이나 테스트 패턴을 선택하세요

#### I2C 명령:
- **Command**: 0x52
- **Data[0]**: 0x02 (LED Enable)

---

### 3. LED 비활성화 (Disable LED)

**메뉴 옵션**: `3`

LED를 끕니다.

#### 사용 방법:
1. 메뉴에서 `3` 입력
2. LED가 자동으로 꺼집니다

#### 예시:
```
Select option: 3
Disabling LED...
I2C Write Success: Command 0x52, 2 bytes written
```

#### I2C 명령:
- **Command**: 0x52
- **Data[0]**: 0x00 (LED Disable)

---

### 4. LED 접합부 온도 읽기 (Read LED Temperature)

**메뉴 옵션**: `4`

LED 칩의 접합부 온도를 측정합니다.

#### 사용 방법:
1. 메뉴에서 `4` 입력
2. 현재 LED 온도가 표시됩니다

#### 예시:
```
Select option: 4
Reading LED junction temperature...
I2C Read Success: 2 bytes read
LED Temperature: 65.30 °C (Raw: 500, Base: 50.00 °C)
```

#### 온도 계산:
```
LED OFF 상태:
  Temperature = (Raw_Value) / 10

LED ON 상태:
  Temperature = (Raw_Value / 10) + (1.6 × 4V × Current_mA / 1000)
```

여기서:
- **1.6**: LED 열저항 (°C/W)
- **4V**: LED 동작 전압
- **Current_mA**: 설정된 LED 전류 (mA)

#### 권장 동작 온도:
- **최대**: 85°C
- **권장**: 60~70°C

#### I2C 명령:
- **Command**: 0xD6
- **Response**: 2 bytes (LSB, MSB)

---

### 5-8. 이미지 플립 설정 (Set Image Flip)

**메뉴 옵션**: `5`, `6`, `7`, `8`

프로젝터 이미지의 방향을 설정합니다.

#### 옵션:
| 메뉴 | 플립 모드 | 설명 |
|------|-----------|------|
| 5 | No Flip | 원본 이미지 (플립 없음) |
| 6 | Horizontal | 좌우 반전 |
| 7 | Vertical | 상하 반전 |
| 8 | Both | 좌우 + 상하 반전 (180° 회전) |

#### 사용 방법:
1. 메뉴에서 원하는 옵션 입력 (5~8)
2. 이미지 방향이 즉시 변경됩니다

#### 예시:
```
Select option: 6
Setting image flip: Horizontal Flip
I2C Write Success: Command 0x14, 2 bytes written
```

#### 사용 시나리오:
- **천장 설치**: 옵션 8 (Both Flip) 사용
- **후면 투사**: 옵션 6 (Horizontal Flip) 사용
- **일반 설치**: 옵션 5 (No Flip) 사용

#### I2C 명령:
- **Command**: 0x14
- **Data[0]**:
  - 0x00: No Flip
  - 0x02: Horizontal
  - 0x04: Vertical
  - 0x06: Both

---

### 9. 광센서 값 읽기 (Read Light Sensor Value)

**메뉴 옵션**: `9`

프로젝터의 광센서 값을 읽습니다.

#### 사용 방법:
1. 메뉴에서 `9` 입력
2. 현재 광센서 값이 표시됩니다

#### 예시:
```
Select option: 9
Reading light sensor value...
Light Sensor Read Success: 2 bytes read
Light Sensor Value: 1523 (0x05F3)
```

#### 광센서 값 해석:
- **값 범위**: 0 ~ 65535
- **높은 값**: 밝은 환경
- **낮은 값**: 어두운 환경

#### 주의사항:
- **고정된 패턴으로 측정하세요**: 패턴에 따라 값이 달라질 수 있습니다
- 권장: Ramp 패턴이나 Checker 패턴 사용

#### I2C 명령:
- **I2C 주소**: 0x39 (다른 주소!)
- **Command**: 0xAC
- **Response**: 2 bytes (LSB, MSB)

---

### 10. HDMI 입력 선택 (Select HDMI Input)

**메뉴 옵션**: `10`

외부 HDMI 입력 소스를 선택합니다.

#### 사용 방법:
1. 메뉴에서 `10` 입력
2. HDMI 입력으로 전환됩니다

#### 예시:
```
Select option: 10
Selecting HDMI input source...
I2C Write Success: Command 0x05, 2 bytes written
```

#### 사용 시나리오:
- 외부 컴퓨터, 게임기, 미디어 플레이어 등의 영상 출력

#### I2C 명령:
- **Command**: 0x05
- **Data[0]**: 0x00 (HDMI Source)

---

### 11. Ramp 테스트 패턴 선택 (Select Ramp Test Pattern)

**메뉴 옵션**: `11`

그라데이션 테스트 패턴을 표시합니다.

#### 사용 방법:
1. 메뉴에서 `11` 입력
2. Ramp 패턴이 표시됩니다

#### 예시:
```
Select option: 11
Selecting Ramp test pattern...
I2C Write Success: Command 0x05, 2 bytes written
I2C Write Success: Command 0x0b, 7 bytes written
```

#### 패턴 설명:
- **Ramp**: 0~255 밝기 그라데이션 패턴
- **용도**: 밝기 균일도, 색상 계조 테스트

#### I2C 명령:
1. **Source Selection**:
   - Command: 0x05
   - Data[0]: 0x01 (Test Pattern)

2. **Pattern Configuration**:
   - Command: 0x0b
   - Data[0]: 0x01 (Ramp)
   - Data[1]: 0x70
   - Data[2]: 0x00
   - Data[3]: 0xff
   - Data[4]: 0x00
   - Data[5]: 0x00

---

### 12. Checker 테스트 패턴 선택 (Select Checker Test Pattern)

**메뉴 옵션**: `12`

체커보드 테스트 패턴을 표시합니다.

#### 사용 방법:
1. 메뉴에서 `12` 입력
2. Checker 패턴이 표시됩니다

#### 예시:
```
Select option: 12
Selecting Checker test pattern...
I2C Write Success: Command 0x05, 2 bytes written
I2C Write Success: Command 0x0b, 7 bytes written
```

#### 패턴 설명:
- **Checker**: 흑백 체커보드 패턴
- **용도**: 픽셀 정렬, 해상도, 포커스 테스트

#### I2C 명령:
1. **Source Selection**:
   - Command: 0x05
   - Data[0]: 0x01 (Test Pattern)

2. **Pattern Configuration**:
   - Command: 0x0b
   - Data[0]: 0x07 (Checker)
   - Data[1]: 0x70
   - Data[2]: 0x04
   - Data[3]: 0x00
   - Data[4]: 0x04
   - Data[5]: 0x00

---

### 0. 종료 (Exit)

**메뉴 옵션**: `0`

프로그램을 종료합니다.

#### 사용 방법:
1. 메뉴에서 `0` 입력
2. USB 장치가 자동으로 닫히고 프로그램이 종료됩니다

#### 예시:
```
Select option: 0
Exiting...
Device closed
```

---

## 기술 사양

### I2C 통신
- **버스 속도**: 100 kHz (I2C Standard Mode)
- **프로토콜**: Philips I2C Specification
- **마스터/슬레이브**: USB Serial이 마스터, DLP가 슬레이브

### LED 사양
- **전류 범위**: 5.86 ~ 6000 mA
- **전류 스텝**: 5.8593 mA
- **동작 전압**: 약 4V
- **열저항**: 1.6°C/W
- **최대 접합부 온도**: 85°C (권장: 60~70°C)

### 타임아웃
- **I2C Read/Write**: 5000 ms (5초)

---

## 일반적인 사용 시나리오

### 시나리오 1: 프로젝터 초기 설정

```
1. HDMI 입력 선택 (메뉴 10)
2. LED 전류 설정 (메뉴 1) → 5000 mA 입력
3. 이미지 플립 설정 (메뉴 5~8) → 설치 방식에 맞게 선택
4. LED 활성화 (메뉴 2)
```

### 시나리오 2: 테스트 패턴으로 테스트

```
1. Ramp 패턴 선택 (메뉴 11)
2. LED 전류 설정 (메뉴 1) → 3000 mA 입력
3. LED 활성화 (메뉴 2)
4. 광센서 값 읽기 (메뉴 9) → 밝기 확인
5. LED 온도 읽기 (메뉴 4) → 열 상태 확인
```

### 시나리오 3: 온도 모니터링

```
1. LED 전류 설정 (메뉴 1) → 5000 mA
2. HDMI 입력 선택 (메뉴 10)
3. LED 활성화 (메뉴 2)
4. 5분 대기
5. LED 온도 읽기 (메뉴 4) → 안정 온도 확인
6. 온도가 70°C 이상이면 전류 감소 고려
```

### 시나리오 4: 밝기 캘리브레이션

```
1. Checker 패턴 선택 (메뉴 12)
2. LED 전류를 3000 mA로 설정 (메뉴 1)
3. LED 활성화 (메뉴 2)
4. 광센서 값 읽기 (메뉴 9) → 기준값 기록
5. LED 전류를 4000 mA로 증가 (메뉴 1)
6. 광센서 값 읽기 (메뉴 9) → 변화량 확인
7. 원하는 밝기에 도달할 때까지 반복
```

---

## 문제 해결

### 1. 장치를 찾을 수 없습니다 (No devices found)

**원인**:
- USB Serial 장치가 연결되지 않음
- 드라이버가 설치되지 않음

**해결 방법**:
1. USB 케이블 연결 확인
2. 장치 관리자에서 Cypress USB Serial 장치 확인
3. 드라이버 재설치

### 2. I2C Write/Read 실패

**원인**:
- I2C 배선 문제
- 잘못된 I2C 주소
- 프로젝터 전원 꺼짐

**해결 방법**:
1. I2C SDA, SCL 배선 확인
2. 프로젝터 전원 확인
3. I2C 풀업 저항 확인 (4.7kΩ)

### 3. LED가 켜지지 않습니다

**원인**:
- 비디오 신호 없음
- LED 전류가 설정되지 않음

**해결 방법**:
1. 먼저 HDMI 입력 또는 테스트 패턴 선택 (메뉴 10~12)
2. LED 전류 설정 (메뉴 1)
3. LED 활성화 (메뉴 2)

### 4. LED 온도가 너무 높습니다 (>80°C)

**원인**:
- LED 전류가 너무 높음
- 냉각 불충분

**해결 방법**:
1. LED 전류 감소 (3000~4000 mA로 낮춤)
2. 프로젝터 냉각 확인 (팬, 히트싱크)
3. 주변 온도 확인

### 5. 광센서 값을 읽을 수 없습니다

**원인**:
- 다른 I2C 주소 사용 (0x39)
- 광센서 미지원 모델

**해결 방법**:
1. DLP 모델 확인 (광센서 지원 여부)
2. I2C 주소 0x39 연결 확인

### 6. 프로그램이 응답하지 않습니다

**원인**:
- I2C 타임아웃
- USB 통신 오류

**해결 방법**:
1. Ctrl+C로 프로그램 강제 종료
2. USB 재연결
3. 프로그램 재시작

---

## 코드 구조

### 주요 함수

```c
// 초기화
CY_RETURN_STATUS InitializeDevice(UINT8 deviceNumber)

// I2C 통신
CY_RETURN_STATUS WriteI2CCommand(UINT8 command, UINT8 *data, UINT32 dataLen)
CY_RETURN_STATUS ReadI2CData(UINT8 command, UINT8 *data, UINT32 dataLen)
CY_RETURN_STATUS ReadLightSensor(UINT8 command, UINT8 *data, UINT32 dataLen)

// LED 제어
CY_RETURN_STATUS SetLEDCurrent(float currentMA)
CY_RETURN_STATUS SetLEDEnable(BOOL enable)
CY_RETURN_STATUS ReadLEDTemperature(float *temperature, BOOL ledOn, float currentMA)

// 이미지 제어
CY_RETURN_STATUS SetImageFlip(UINT8 flipMode)

// 센서
CY_RETURN_STATUS ReadLightSensorValue(UINT16 *value)

// 입력 소스
CY_RETURN_STATUS SelectHDMIInput(void)
CY_RETURN_STATUS SelectRampPattern(void)
CY_RETURN_STATUS SelectCheckerPattern(void)
```

---

## 참고 자료

### 파일 설명
- **CyTest.c**: 메인 프로그램 소스 코드
- **CyUSBSerial.h**: Cypress USB Serial 라이브러리 헤더
- **DLPdoc.c**: DLP I2C 명령 매뉴얼
- **cyusbserial.dll**: Cypress USB Serial 동적 라이브러리
- **cyusbserial.lib**: 링크용 라이브러리 파일

### 관련 문서
- Cypress USB Serial Library Documentation
- DLP Projector I2C Command Specification
- Philips I2C Specification

---

## 라이선스

본 프로그램은 Cypress USB Serial Library를 사용하며, 해당 라이브러리의 라이선스를 따릅니다.

---

## 문의

프로그램 사용 중 문제가 발생하거나 질문이 있으시면:
- 이슈 리포트 작성
- 기술 문서 참조
- 제조사 지원팀 연락

---

**마지막 업데이트**: 2025-10-23