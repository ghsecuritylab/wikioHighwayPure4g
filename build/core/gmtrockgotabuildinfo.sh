#!/bin/bash

if [ $# -lt 1 ] ; then
echo "Usage: $0 build.prop"
echo "hufeng.mao@generalmobi.com"
exit
fi

echo ""
echo "# begin ROCK GOTA properties"

# true|false,用于手动配置brand与model的开关
GMT_MANUAL_SET=false

# 当GMT_MANUAL_SET宏为true时有用
LOCAL_PROJECT_BRAND_VALUE="Fly"
LOCAL_PROJECT_MODEL_VALUE="IQ 4412"

# 根据项目配置以下内容
LOCAL_PROJECT_BRAND="ro.product.manufacturer"
LOCAL_PROJECT_MODEL="ro.product.model"
LOCAL_PROJECT_VERSION="ro.build.display.id"

# DO NOT MODIFY!不要修改这里的值
ROCK_GOTA_BRAND="ro.rock.gota.brand"
ROCK_GOTA_MODEL="ro.rock.gota.model"
ROCK_GOTA_VERSION="ro.rock.gota.version"

if [ "$GMT_MANUAL_SET" == "true" ] ; then
	echo "$ROCK_GOTA_BRAND=$LOCAL_PROJECT_BRAND_VALUE"
	echo "$ROCK_GOTA_MODEL=$LOCAL_PROJECT_MODEL_VALUE"
else
	echo "$(grep "$LOCAL_PROJECT_BRAND" "$1" | sed "s/$LOCAL_PROJECT_BRAND/$ROCK_GOTA_BRAND/" )"
	echo "$(grep "$LOCAL_PROJECT_MODEL" "$1" | sed "s/$LOCAL_PROJECT_MODEL/$ROCK_GOTA_MODEL/" )"
fi

echo "$(grep "$LOCAL_PROJECT_VERSION" "$1" | sed "s/$LOCAL_PROJECT_VERSION/$ROCK_GOTA_VERSION/" )"_`date +%s`
# echo "$grep "ro.build.display.id=" "$1" | sed "s/ro. Radium/3G7334i/" "

echo "# end ROCK GOTA properties"
