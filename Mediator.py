import os
import platform
import stat
import subprocess
import sys

domyslny_limit_czasu = 5  # Domyślny limit czasu dla mediatora

def ustaw_uprawnienia_wykonania(sciezka_pliku):
    aktualny_system = platform.system()  # Sprawdź aktualny system operacyjny

    if aktualny_system == 'Linux':
        os.chmod(sciezka_pliku, stat.S_IRWXU)  # Ustaw uprawnienia wykonania dla właściciela (użytkownika) na Linuxie
    elif aktualny_system == 'Windows':
        # Dodaj kod do ustawiania uprawnień wykonania dla pliku na systemie Windows, jeśli jest to potrzebne
        pass
    else:
        print("Nieobsługiwany system operacyjny.")

def uruchom_mediator(plik1, plik2, plik3, plik4):
    nazwa_programu = "play"  # Nazwa programu C++
    build_folder=os.getcwd()
    sciezka_programu = os.path.join(build_folder, nazwa_programu)
    # Sprawdź, czy plik programu istnieje
    if os.path.exists(sciezka_programu):
        # Uruchom program C++ jako proces podrzędny
        subprocess.call([sciezka_programu, plik1, plik2, plik3, plik4])
    else:
        print("Program C++ nie został znaleziony.")

if __name__ == "__main__":
    # Ustaw uprawnienia wykonania dla skryptu mediatora
    sciezka_skryptu = os.path.realpath(__file__)  # Pobierz pełną ścieżkę do skryptu mediatora program_name = "play"
    #print(sciezka_skryptu)
    if len(sys.argv) < 4:
        print("Proszę podać co najmniej trzy argumenty tekstowe.")
        sys.exit(1)

    arg1 = sys.argv[1]
    arg2 = sys.argv[2]
    arg3 = sys.argv[3]

    if len(sys.argv) >= 5:
        try:
            arg4 = int(sys.argv[4])
        except ValueError:
            print("Czwarty argument musi być liczbą całkowitą.")
            sys.exit(1)
        uruchom_mediator(arg1, arg2, arg3, str(arg4))
    else:
        uruchom_mediator(arg1, arg2, arg3, str(domyslny_limit_czasu))

