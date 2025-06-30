// // #include "sslworker.h"
// #include <openssl/err.h>

// // SslWorker::SslWorker()
// // {
// // //    SSL_load_error_strings();
// // //    OpenSSL_add_ssl_algorithms();
// // //    rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
// // }

// // SSL_CTX *SslWorker::create_ssl_context()
// // {
// //    // Инициализировать OpenSSL
// //    SSL_library_init(); //-` инициализирует библиотеку OpenSSL. Это делает доступными алгоритмы шифрования, дайджестов и SSL. Эта функция должна быть вызвана перед любыми другими операциями SSL/TLS.
// //    SSL_load_error_strings(); //   загружает читаемые человеком описания ошибок, так что если что-то пойдет не так, вы сможете получить понятное сообщение об ошибке.
// //    OpenSSL_add_all_algorithms();  // добавляет все алгоритмы шифрования и дайджестов в таблицу внутри OpenSSL, делая их доступными для SSL/TLS.

// //    const SSL_METHOD* method = TLS_method();
// // //    const SSL_METHOD* method = SSLv23_method(); // Можно использовать TLS_method() для TLS
// //    SSL_CTX* ctx = SSL_CTX_new(method);         //оздает новый контекст SSL/TLS, который определяет настройки для создаваемых SSL/TLS соединений. Параметр, принимаемый этой функцией, определяет методы протокола (например, TLS).
// //    if (!ctx) {
// //        std::cout << stderr;
// // //        ERR_print_errors_cb(stderr);
// // //        ERR_print_errors_fp(stderr);
// //        return nullptr;
// //    }

// //    return ctx;
// // }

// // bool SslWorker::configure_ssl_context(SSL_CTX *ctx)
// // {
// //    // Задать список cipher suites, который включает ECC и AES-GCM
// //    if (SSL_CTX_set_cipher_list(ctx, "ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384") != 1) // настраивает список шифров, которые будут использоваться во всех соединениях, созданных с использованием данного контекста. Это управляет выбором шифра во время TLS-рукопожатия. Например, "ECDHE-ECDSA-AES256-GCM-SHA384" указывает использование ECDHE для ключевого обмена, ECDSA как метода аутентификации, AES256-GCM как алгоритма шифрования и SHA384 как функции хэширования.
// //    {
// //        std::cout << stderr;
// //        return false;
// //    }

// //    // Установка параметров для использования эллиптических кривых
// //    SSL_CTX_set1_curves_list(ctx, "P-256:P-384:P-521"); //Эта функция позволяет задать список эллиптических кривых, которые будут использоваться для ECDH/ECDHE в процессе рукопожатия. Использование этой функции важно для тонкой настройки производительности и безопасности.

// //    return true;
// // }

// // SSL *SslWorker::prepare_ssl_connection(SSL_CTX *ctx, int client_socket)
// // {
// //    SSL* ssl = SSL_new(ctx); // оздает новый SSL объект на основе контекста. Этот объект используется для установления SSL/TLS соединения.
// //    if (!ssl) {
// //        std::cout << stderr;
// //        return nullptr;
// //    }

// //    SSL_set_fd(ssl, client_socket); // связывает SSL объект с конкретным файловым дескриптором (например, сокетом). Это позволяет SSL объекту использовать сокет для отправки и получения данных через SSL/TLS соединение.

// //    return ssl;
// // }

// // #include <QTcpSocket>

// // void SslWorker::test()
// // {
// //    SSL_library_init();
// //    SSL_load_error_strings();
// //    OpenSSL_add_all_algorithms();

// //    // Создание QTcpSocket
// //    QTcpSocket *socket = new QTcpSocket();

// //    // Подключение к серверу
// //    socket->connectToHost("127.0.0.1", 1234); // IP-адрес и порт netcat сервера

// //    // Создание SSL контекста
// //    SSL_CTX* ctx = SSL_CTX_new(SSLv23_client_method());
// //    SSL* ssl = SSL_new(ctx);

// //    // Привязка QTcpSocket к SSL
// //    SSL_set_fd(ssl, socket->socketDescriptor());

// //    // Установка SSL соединения
// //    if (SSL_connect(ssl) != 1) {
// //        qDebug() << "Error establishing SSL connection";
// //        return;
// //    }

// //    // Обработка событий
// //    QObject::connect(socket, &QTcpSocket::readyRead, [=](){
// //        char buffer[1024];
// //        int bytesRead = SSL_read(ssl, buffer, sizeof(buffer));
// //        qDebug() << "Data received: " << QByteArray(buffer, bytesRead);
// //    });
// // }
