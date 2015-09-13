#!/system/bin/sh
. ./setup_variables.sh

if [ z"$DEVICE_ID" != z ]; then
  echo "Export address to '$FILE'"
  SQL="select name, value from device_address where device_id=$DEVICE_ID;"
  echo "$SQL" | "$SQLITE3" "$DEVICE_DB" > "$FILE"
  exit $?
else
  exit 1
fi
