USER="pi"
IP="10.0.0.112"
PASS="raspberry"
TARGET_PATH="~/rusty_bot/raspPins/TheSpaceShip/target/armv7-unknown-linux-gnueabihf/debug/TheSpaceShip"

sshpass -p $PASS scp $TARGET_PATH $USER@$IP:~

echo $TARGET_PATH $USER@$IP:~