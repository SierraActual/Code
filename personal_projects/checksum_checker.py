import hashlib
import PySimpleGUI as sg

def calculate_sha256(file_path):
    sha256_hash = hashlib.sha256()

    with open(file_path, "rb") as f:
        # Read file in chunks of 4K
        for byte_block in iter(lambda: f.read(4096), b""):
            sha256_hash.update(byte_block)

    return sha256_hash.hexdigest()

if __name__ == "__main__":
    layout = [
        [sg.Text('Select the file:'), sg.InputText(key='-FILE_PATH-'), sg.FileBrowse()],
        [sg.Text('Enter the checksum string to compare:'), sg.InputText(key='-CHECKSUM-')],
        [sg.Button('Check Sums'), sg.Button('Exit')]
    ]

    window = sg.Window('Checksum Calculator', layout)

    while True:
        event, values = window.read()

        if event == sg.WIN_CLOSED or event == 'Exit':
            break

        if event == 'Check Sums':
            file_path = values['-FILE_PATH-']
            checksum_input = values['-CHECKSUM-']

            try:
                calculated_checksum = calculate_sha256(file_path)

                if calculated_checksum == checksum_input:
                    sg.popup("Checksums match!")
                else:
                    sg.popup("Checksums do not match!")
            except FileNotFoundError:
                sg.popup("File not found. Please provide a valid file path.")
            except Exception as e:
                sg.popup(f"An error occurred: {e}")

    window.close()
