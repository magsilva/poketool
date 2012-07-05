#
# pokeconfigure -- shell script to prepare for poketool building
#

# Setting up environment varables

ARCH=`arch`
#VERSION=`date +%F`
VERSION="SVN"
DIRPOKE=`pwd`

export ARCH DIRPOKE 

# Creating directories for binaries, scripts and tables

if ! test -d bin
then 
   mkdir bin
fi

if ! test -d bin/litabs
then 
   mkdir bin/litabs
fi

if ! test -d bin/poketabs
then 
   mkdir bin/poketabs
fi

# Building poketool modules

MODULES="li chanomat lib descr pokernel avaliador heuristic"

echo "Building modules: $MODULES"
for mod in $MODULES
do
  echo ""
  echo "==> Building $mod module"
  echo ""
  cd $DIRPOKE/$mod
  if ! test -d $ARCH
  then 
     mkdir $ARCH
  else
      rm -f $ARCH/*
      rm -f lib*.a
      rm -f $mod
      rm -f new$mod
  fi
  make -f makefile.gcc
  cd $DIRPOKE
done

# Copy scripts utilized by poketool

echo ""
echo "==> Copying scripts"
echo ""

echo "cp $DIRPOKE/pokescripts/newpoketool $DIRPOKE/pokescripts/newpokeaval $DIRPOKE/pokescripts/pokeexec $DIRPOKE/pokescripts/pokebatch $DIRPOKE/bin"
cp $DIRPOKE/pokescripts/newpoketool $DIRPOKE/pokescripts/newpokeaval $DIRPOKE/pokescripts/pokeexec $DIRPOKE/pokescripts/pokebatch $DIRPOKE/bin

echo ""
echo "==> Creating distribution"
echo ""


# Creating directories for creating the distribuition

cd $DIRPOKE

if ! test -d distrib
then 
   mkdir distrib
else
   cd distrib
   rm -rf bin docs
fi

cd $DIRPOKE/distrib

if ! cp -r $DIRPOKE/bin .
then
  echo "Poketool distribution generation for [$ARCH] failed." 
fi

if cp -r $DIRPOKE/docs .
then
  rm -rf $DIRPOKE/docs/CVS
else
  echo "Poketool distribution generation for [$ARCH] failed." 
fi

if tar czf poke-$ARCH-$VERSION.tgz bin docs
then 
  echo "Poketool distribution generation for [$ARCH] in 'distrib' was successful."
else
  echo "Poketool distribution generation for [$ARCH] failed." 
fi

# Clean up

#rm -rf $DIRPOKE/distrib/docs/*
#rm -rf $DIRPOKE/distrib/bin/*
