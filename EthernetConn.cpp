#include <SPI.h>
#include <Ethernet.h>
#include "EthernetConn.h"
#include "Smart-Garden.h"
#include "SDLog.h"

IPAddress MyIp(192, 168, 1, 177);

byte MyMac[] = 
{
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

EthernetServer GeneralServer(80);
EthernetClient GeneralClient;

void EthInit()
{
	uint8_t ReTryCnt = 5;
	bool NoShieldConnected = false, EthNotDHCP = false;
	Ethernet.init(ETH_CS);
	if(Ethernet.hardwareStatus() != EthernetNoHardware)
		NoShieldConnected = false;
	else
		NoShieldConnected = true;
	if(!NoShieldConnected)
	{
		DBG("Task ETH_SD: Shield connessa");
		while(!Ethernet.begin(MyMac))
		{
			ReTryCnt--;
			if(ReTryCnt == 0)
			{
				EthNotDHCP = true;
				break;
			}
			EthNotDHCP = false;
			delay(1000);
		}
		if(EthNotDHCP)
		{
			Ethernet.begin(MyMac, MyIp);
			DBG("Task ETH_SD: DHCP non trovato");
		}
		else
			SystemFlag.EthCableConnected = true;
	}
	else
	{
		DBG("Task ETH_SD: Shield non connessa");
	}
	delay(1000);
}

void CheckCableConnection()
{
	if(!SystemFlag.EthClient)
	{
		EthernetClient ClientForCheck;
		char *ServerName = "www.google.it";
		if(ClientForCheck.connect(ServerName, 80))
		{
			SystemFlag.EthCableConnected = true;
			ClientForCheck.stop();
			GeneralServer.begin();
			DBG("Task Eth-SD, ip server: ");
			DBG(Ethernet.localIP());		
		}
		else
			SystemFlag.EthCableConnected = false;
	}
}

void EthServer()
{
	if(SystemFlag.EthCableConnected && !SystemFlag.SDLogging)
	{
		GeneralClient = GeneralServer.available();
		if (GeneralClient) 
		{
			// an http request ends with a blank line
			bool currentLineIsBlank = true;
			if(GeneralClient.connected()) 
			{
				SystemFlag.EthClient = true;
				if (GeneralClient.available()) 
				{
					char c = GeneralClient.read();
					// if you've gotten to the end of the line (received a newline
					// character) and the line is blank, the http request has ended,
					// so you can send a reply
					if (c == '\n' && currentLineIsBlank) 
					{
						// send a standard http response header
						GeneralClient.println("HTTP/1.1 200 OK");
						GeneralClient.println("Content-Type: text/html");
						GeneralClient.println("Connection: close");  // the connection will be closed after completion of the response
						// GeneralClient.println("Refresh: 5");  // refresh the page automatically every 5 sec
						GeneralClient.println();
						GeneralClient.println("<!DOCTYPE HTML>");
						GeneralClient.println("<html>");
						if(SystemFlag.SDWebPagePresent)
						{
							WriteWebPageToClient(); //GeneralClient.println("Eth webpage"); // Mettere la pagina creata e salvata su SD
						}
						else
							GeneralClient.println("Index not found!");
						GeneralClient.println("</html>");
					}
					if (c == '\n') 
					{
						// you're starting a new line
						currentLineIsBlank = true;
					} 
					else if (c != '\r') 
					{
						// you've gotten a character on the current line
						currentLineIsBlank = false;
					}
				}
			}
			else
			{
				SystemFlag.EthClient = false;
				GeneralClient.stop();
			}
		}
	}
}

// const char WebPage[] PROGMEM  = 
// {
// "Utilizzo dei nostri Servizi È necessario seguire tutte le norme messe a disposizione dell\'utente all\'interno dei Servizi.I nostri Servizi non devono essere utilizzati in modo improprio. Ad esempio, è vietato interferire con i nostri Servizi o tentare di accedervi utilizzando un metodo diverso dall\'interfaccia e dalle istruzioni da noi fornite. È possibile utilizzare i nostri Servizi solo nei modi consentiti dalla legge, incluse le leggi e le normative vigenti sui controlli delle esportazioni e delle riesportazioni. Potremmo sospendere o interrompere la fornitura dei nostri Servizi all\'utente qualora questi non rispettasse i nostri termini o le nostre norme oppure qualora stessimo effettuando accertamenti su un caso di presunto comportamento illecito.Con l\'utilizzo dei nostri Servizi, l\'utente non diventa titolare di alcun diritto di proprietà intellettuale sui nostri Servizi o sui contenuti a cui accede. È vietato utilizzare i contenuti provenienti dai nostri Servizi senza aver ottenuto l\'autorizzazione dal proprietario o ove non sia consentito dalla legge. I presenti termini non concedono all\'utente il diritto di utilizzare alcun marchio o logo impiegato nei nostri Servizi. È vietato rimuovere, oscurare o alterare eventuali note legali visualizzate nei nostri Servizi o insieme a essi.Nei nostri Servizi vengono visualizzi contenuti che non sono di proprietà di Google. Tali contenuti sono esclusiva responsabilità del soggetto che li rende disponibili. Potremmo riservarci il diritto di esaminare i contenuti per stabilirne l\'eventuale illegalità o contrarietà alle nostre norme, e potremmo altresì rimuovere o rifiutarci di visualizzare dei contenuti qualora avessimo ragionevole motivo di ritenere che violino le nostre norme o la legge. Ciò non significa necessariamente che esaminiamo i contenuti, né lo si potrà presumere.In relazione all\'utilizzo dei Servizi da parte dell\'utente, potremmo inviare a quest\'ultimo annunci di servizio, messaggi amministrativi e altre informazioni. È possibile disattivare l\'invio di alcune comunicazioni.Alcuni dei nostri Servizi sono disponibili sui dispositivi mobili. Tali Servizi non devono essere utilizzati qualora possano distrarre e causare violazioni del codice della strada o delle leggi sulla sicurezza.L\'account Google dell\'utentePer utilizzare alcuni dei nostri Servizi potrebbe essere necessario un account Google. L\'account Google può essere creato dall\'utente stesso o assegnato da un amministratore, ad esempio un datore di lavoro o un istituto scolastico. Qualora l\'utente utilizzasse un account Google assegnato da un amministratore, potrebbero essere applicati termini diversi o aggiuntivi e l\'amministratore potrebbe essere in grado di accedere all\'account o di disattivarlo.Per proteggere l\'account Google, è necessario mantenere la password riservata. L\'utente è responsabile dell\'attività svolta su o attraverso il proprio account Google. È consigliabile non riutilizzare la password dell\'account Google per applicazioni di terze parti. In caso si venisse a conoscenza di qualsiasi utilizzo non autorizzato della propria password o del proprio account Google, seguire queste istruzioni.Protezione della privacy e del copyrightLe Norme sulla privacy di Google illustrano le modalità di trattamento dei dati personali e di tutela della privacy dell'utente nel contesto dell'utilizzo dei nostri Servizi.Rispondiamo alle notifiche di presunta violazione del copyright e provvediamo a chiudere gli account dei trasgressori recidivi in conformità con la procedura stabilita nel Digital Millennium Copyright Act (Legge statunitense sul copyright).Forniamo informazioni per aiutare i titolari di copyright a gestire la loro proprietà intellettuale online. Qualora l\'utente ritenesse che qualcuno stia violando il suo copyright e desiderasse segnalarcelo, può trovare informazioni sull\'invio delle notifiche e sulle norme di Google in materia di risposte alle notifiche nel nostro Centro assistenza."

// };
