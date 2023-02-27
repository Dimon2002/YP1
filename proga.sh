#Если файл с таким названием существует и является каталогом
if [ -e $1 ] && [ -d $1 ]; then

  if [ -r $1 ]; then
  
    #Переходим в этот каталог
    cd $1 2>sherror.txt
  
    #Оформляем рамку таблицы вывода
    echo '========================'
    echo 'N   Name Directory'
  
    #Создаем переменную-счетчик для количества подходящих подкаталогов
    counter=1

    #Записываем в lists названия всех подкаталогов текущего каталога
    lists="$(ls -F|grep '/'|sed 's/\///')"
  
    #Для каждого подкаталога
    for element in $lists

      do

         #Переходим в этот подкаталог
         cd $element 2>sherror.txt

         #Подсчитываем количество каталогов в текущем подкаталоге
         insidenumber="$(ls -F|grep '/'|wc -w)"

         #Если количество каталогов в текущем подкаталоге равно 0,
         #то есть в текущем подкаталоге нет каталогов
         if [ $insidenumber -eq 0 ]; then

           #То выводим название подкаталога
           echo '['$counter']' $element

           #Увеличиваем значение счетчика на единицу
           counter=$((counter+1))

         fi

         #Возвращаемся в каталог на уровень выше, в котором происходит поиск
         cd ..

     done

     #Оформляем рамку вывода таблицы
     echo '========================='

  else
   
   #echo 'You have no right!'
   exec 2> sherror.txt

  fi

#Если же не существует каталога с введенным названием
else

  #Выводится сообщение об ошибке
  #echo "There is no such directory"
  echo $ENOENT

fi
