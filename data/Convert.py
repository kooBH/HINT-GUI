import numpy as np
import librosa as rs
import soundfile as sf
import glob,os
from tqdm import tqdm



list_input = glob.glob(os.path.join("SNR30_float","*.wav"))

os.makedirs("SNR30_int16",exist_ok=True)
for path in tqdm(list_input):
    data, samplerate = rs.load(path, sr=16000, mono=True)
    data = (data * 32767).astype(np.int16)  # Convert to int16
    output_path = os.path.join("SNR30_int16", os.path.basename(path))
    sf.write(output_path, data, samplerate=16000, subtype="PCM_16")