#!/system/bin/sh
NAME="$1"
VALUE="$2"

. ./setup_variables.sh

if [ z"$DEVICE_ID" != z ]; then
  SQL="insert into device_address(device_id, name, value) values($DEVICE_ID, '$NAME', $VALUE);"
  echo "$SQL" | "$SQLITE3" "$DEVICE_DB"
  exit $?
else
  exit 1
fi
