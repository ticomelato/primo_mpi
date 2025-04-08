📦 Clonar o projeto
git clone https://github.com/ticomelato/primo_mpi
cd primo_mpi


📁 Arquivos
primo_mpi.c: Código-fonte principal que executa o cálculo de primos.

hostfile: Arquivo de configuração contendo os IPs/nomes das máquinas que serão usadas no MPI (exemplo abaixo).

🛠️ Compilação
Compile o programa com mpicc:
  mpicc -o primos primo_mpi.c -lm

🚀 Execução em ambiente distribuído
1. Configure o arquivo hostfile
  Substitua pelos IPs reais das suas VMs/máquinas.

2. Copie o programa para a segunda máquina
  Use o scp para enviar o binário para a segunda máquina:
  scp primos mpiuser@192.168.50.11:/home/mpiuser/
  Altere o IP e o caminho de acordo com sua rede e usuário.

3. Execute com mpirun
  mpirun -np 2 -hostfile hostfile ./primos
    -np 2: Número de processos (um por máquina no exemplo).
    -hostfile hostfile: Caminho do arquivo com os hosts.

📌 Observações
Certifique-se de que as máquinas podem se comunicar via SSH sem senha (configuração de chave SSH).
É importante que o nome de usúario seja o mesmo nas duas máquinas.

O programa atualmente calcula todos os números primos até 100000, mas você pode alterar o valor da variável limite no código-fonte.

Resultados
Você deve esperar um resultado semelhante à este:
![image](https://github.com/user-attachments/assets/6497bdca-b384-4a04-8423-a20655aedb27)

