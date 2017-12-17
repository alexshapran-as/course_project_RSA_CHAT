
Rsa rsa; // объявление переменной класса RSA (делают оба человека)

// генерация ключей и шифрование сообщения
Key keys = rsa.produce_keys(); // генерация ключей (делает только тот, кто принимает)

// ......отправка ключей.........

//    long crypt = 9635; // сообщение 
//    crypt = rsa.endecrypt(crypt,keys.ekey,keys.pkey); // шифрование 
// 						    (сообщение, экспонента, публичный ключ) 

		// Вывод ключей на экран
//    qDebug() << "public key: " << keys.pkey; 
//    qDebug() << "exp key: " << keys.ekey;
//    qDebug() << "private key: " << keys.dkey;
		// вывод зашифрованного сообщения
//    qDebug() << crypt;

// ...отправка зашифрованного сообщения...

crypt = rsa.endecrypt(crypt,keys.dkey,keys.pkey); // расшифровка 
//                    (зашифр. сообщение, приватный, публичный)
qDebug() << crypt; // вывод

