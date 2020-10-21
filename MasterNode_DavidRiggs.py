#Written for a Raspberry Pi 4

import os
import time
import serial
serial_port = serial.Serial('/dev/ttyS0', 9600)

#Splits the players into victim+team and shooter+team
def playerSplit(signal):
    return divmod(signal, 0x100)
    
#Splits player from their team    
def playerTeamSplit(player):
    return divmod(player, 0X10)
    
life = 500
teamList = [0]*100
playerList = [0]*100
hitScore = 50 #hit penalty
playerList[0] = life
playerList[1] = life

speech = f'WELCOME TO INFINITE LASER TAG!! Attrition Edition.'
os.system("echo " + speech + "| festival --tts")

while True:
    receivedCode = serial_port.readline().strip()
    
    if playerList[0] == (life/2) or playerList[1] == (life/2):
        speech = f'Player {victim} at half life! Finish Him!'
        os.system("echo " + speech + "| festival --tts")
   
    if len(receivedCode) > 1:
        victimTeam = receivedCode[1]
        shooterTeam = receivedCode[0]
  
        victim, victimT = playerTeamSplit(victimTeam)
        shooter, shooterT = playerTeamSplit(shooterTeam)
        
        if (victim == shooter):
            print('\nPlayer', victim, 'shot themselves!!')
            speech = f'Player {victim} shot themselves!'
            os.system("echo " + speech + "| festival --tts")
            playerList[shooter] = playerList[shooter] - hitScore;
            teamList[shooterT] = teamList[shooterT] - hitScore;
            if (teamList[shooterT] < 0):
                teamList[shooterT] = 0;
            elif (playerList[shooter] < 0):
                playerList[shooter] = 0;
        
        else:
            playerList[victim] = playerList[victim] - hitScore
            teamList[victimT] = teamList[victimT] - hitScore
            print('\nPlayer', victim, 'was shot');
        
        print(f'Player {victim} score is: {playerList[victim]}.')
        
    if (playerList[0] <= 0 or playerList[1] <= 0):
        if (playerList[0] == 0):
            speech = f'Player 1 is the winner!'
            os.system("echo " + speech + "| festival --tts")
        else:
            speech = f'Player 0 is the winner!'
            os.system("echo " + speech + "| festival --tts")
            
        break;
serial_port.close()

