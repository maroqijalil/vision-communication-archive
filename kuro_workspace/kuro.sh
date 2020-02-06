if python3 ${KURO_PATH}/python/kuro.py $@
then
  if [ -f /tmp/kuro.sh ]; then
    source /tmp/kuro.sh
  fi

  return 0
fi

return 1