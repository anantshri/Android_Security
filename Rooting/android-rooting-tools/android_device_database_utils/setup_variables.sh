BUSYBOX=/data/local/tmp/busybox
SQLITE3=./sqlite3

if [ ! -x "$BUSYBOX" ]; then
  echo "Please install busybox to '"$BUSYBOX"'."
  exit 1
fi

if [ ! -x "$SQLITE3" ]; then
  echo "Please install sqlite3 to '"$SQLITE3"'."
  exit 1
fi

DEVICE_DB=device.db
if [ ! -w "$DEVICE_DB" ]; then
  DEVICE_DB=/data/local/tmp/device.db
  if [ ! -w "$DEVICE_DB" ]; then
    echo "Make sure install \"device.db\" from device_database!"
    exit 1
  fi
fi

DEVICE=`getprop ro.product.model`
BUILD_ID=`getprop ro.build.display.id`

if [ z"$CHECK_PROP_NAME" == z ]; then
  SQL="select check_property_name from supported_devices \
      where device='$DEVICE' and build_id='$BUILD_ID' \
      and check_property_name is not null;"

  CHECK_PROP_NAME=`echo "$SQL" | "$SQLITE3" "$DEVICE_DB" | "$BUSYBOX" head -n 1`
fi

if [ z"$CHECK_PROP_NAME" == z ]; then
  FILE="$DEVICE"_"$BUILD_ID".txt
  CHECK_COND='check_property_name is null'
  CHECK_VALUES='null, null'
else
  CHECK_PROP_VALUE=`getprop $CHECK_PROP_NAME`

  #echo "Extra check property: $CHECK_PROP_NAME = $CHECK_PROP_VALUE"

  FILE="$DEVICE"_"$BUILD_ID"_"$CHECK_PROP_NAME"_"$CHECK_PROP_VALUE".txt
  CHECK_COND="check_property_name='$CHECK_PROP_NAME' and check_property_value='$CHECK_PROP_VALUE'"
  CHECK_VALUES="'$CHECK_PROP_NAME', '$CHECK_PROP_VALUE'"
fi

SQL="select device_id from supported_devices \
    where device='$DEVICE' and build_id='$BUILD_ID' \
    and $CHECK_COND;"

DEVICE_ID=`echo "$SQL" | "$SQLITE3" "$DEVICE_DB"`

if [ z"$DEVICE_ID" == z ]; then
  DEVICE_ID=10000

  while :
  do
    SQL="insert into supported_devices(device_id, device, build_id, \
        check_property_name, check_property_value) \
        values ($DEVICE_ID, '$DEVICE', '$BUILD_ID', $CHECK_VALUES); "

    echo "$SQL" | "$SQLITE3" "$DEVICE_DB" 2>setup_variables.log && break

    DEVICE_ID=`expr $DEVICE_ID + 1`

    if [ $DEVICE_ID == 10099 ]; then
       exit 1
    fi
  done
fi
