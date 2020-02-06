FILE=${KURO_PATH}'/install/lib/'$1'/'$2'.py'

if [ -f $FILE ]
then
  COMMAND='python3 '$FILE' | tee '$3
else
  COMMAND='ros2 run '$1' '$2' | tee '$3
fi

if $COMMAND
then
  RET=0
else
  RET=1
fi

echo ""
echo "end of the executable, press ctl+c to exit"

while true
do
  sleep 30
done

return $RET
