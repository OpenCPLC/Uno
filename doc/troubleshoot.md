




Debuger skończył pracę na `HardFault_Handler`.


- Zbyt mały stos `stack` pamięci przypisany do jednego z wątków.
- Błąd pamięci







```
undefined reference to `thread'
undefined reference to `VRTS_Init'
```

Ustaw wartość definicji `VRTS_SWITCHING` na `1` w pliku `main.h` 

#define VRTS_SWITCHING 1







Brak obsługi przycisku
Brak migającej diody
Innyych standardowych funkcji, które dostaracz wątek PLC

- Brak lub skrajnie żadnie wywoływanie funkcji zwalnjającej rdzeń w naszym wątku, takiej jaki let() czy delay()

rozwiązanie:

dodaj funkcję let() na końcu pętli while(1) w dwoim wątku