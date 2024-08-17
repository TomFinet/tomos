while test $# != 0
do 
  case "$1" in
    -d|--debug)
      DEBUG_ON='-S -s';;
  esac
  shift
done

SCRIPTDIR=$(dirname "$BASH_SOURCE")
echo $SCRIPTDIR

TOMOS_QEMU="qemu-system-i386"
TOMOS_IMG="tomos.img"

TOMOS_QEMU_ARGS="
  -machine pc
  -accel tcg
  -no-reboot
  -drive file=$TOMOS_IMG,cache=directsync,format=raw,id=disk
  -m 512M
  -serial mon:stdio
  ${DEBUG_ON}"

${TOMOS_QEMU} ${TOMOS_QEMU_ARGS}
