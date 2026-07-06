# Implementação

>[!NOTE] 
# Ambiente de Desenvolvimento e Ferramentas 

   > Linguagem de Programação: C++ (padrão C++17), escolhido pela sua alta performance, controle de memória e compatibilidade nativa com manipulação de hardware e fluxos de dados seriais.
   > Framework Principal: Qt Framework (versão Qt 6), utilizado para a construção e gerenciamento de toda a interface gráfica lúdica e reativa do sistema (Smart Home Dashboard).
   > IDE de Desenvolvimento: Qt Creator, ambiente integrado utilizado para a codificação, compilação de binários e análise de desempenho em tempo real.
    Compilador: MinGW (Minimalist GNU for Windows), responsável por traduzir o código-fonte C++ em instruções de máquina executáveis.

#Módulos e Bibliotecas Utilizadas
   > QT += widgets: Módulo essencial do Qt utilizado para instanciar componentes estruturais da interface, tais como QWidget, QLabel, QPushButton, QTextEdit e o sistema de gerenciamento de layouts e empilhamento dinâmico (QVBoxLayout, QHBoxLayout, QStackedLayout).
   > QT += serialport (QSerialPort & QSerialPortInfo): Biblioteca nativa do Qt fundamental para atender aos requisitos de comunicação. É responsável por fazer a varredura ativa das portas físicas (COM) do notebook em segundo plano e configurar os parâmetros de barramento de dados (como BaudRate 9600, DataBits, Parity e StopBits) para coletar as strings dos sensores.
   > QTimer: Biblioteca de temporização assíncrona baseada em milissegundos. Foi implementada em duas frentes independentes: uma dedicada a atualizar as informações e simulações climáticas rigidamente a cada 10 segundos, e outra focada em monitorar a presença e integridade do sinal USB a cada 3 segundos.
   > QPainter: Biblioteca de desenho gráfico vetorial em baixo nível. Utilizada de forma customizada na classe CompassWidget para renderizar matematicamente a bússola meteorológica, calculando os pontos cardinais e a rotação em graus polares do ponteiro azul indicador.
   > QRandomGenerator: Utilizado para emular de forma pseudoaleatória o comportamento real da atmosfera (como rajadas de vento, índices pluviométricos e oscilações de temperatura) na ausência de dados lidos pelo hardware.
   > QFileDialog & QTextStream: Bibliotecas responsáveis pela manipulação e persistência de arquivos no sistema operacional, permitindo que o operador exporte o histórico de dados lidos em formato de texto plano (.txt) diretamente no dispositivo local.>

<div align="center">

[Retroceder](projeto.md) | [Avançar](testes.md)

</div>
