#!/system/bin/sh
. ./setup_variables.sh

if [ z"$DEVICE_ID" != z ]; then
  SQL="select * from device_address where device_id=$DEVICE_ID;"
  echo "$SQL" | "$SQLITE3" "$DEVICE_DB"
  exit $?
else
  exit 1
fi
