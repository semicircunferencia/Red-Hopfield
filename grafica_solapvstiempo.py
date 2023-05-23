import matplotlib
import matplotlib.pyplot as plt
import numpy as np

plt.rcParams["font.family"] = "DejaVu Sans"
#fig = plt.figure(figsize=(11.,8.))      # figura (ancho, largo)
ax = plt.subplot()      # subfigura

# Datos
data=[0]*8
for i in range(8):
    data[i]=np.loadtxt(f"T={i+1}.dat")
    # data[i]=data[i][:,1] # esto es para varios patrones

tiempo = np.linspace(0,100,101) # cojo el tiempo de la primera columna

# configurar ejes
ax.set_ylabel('Solapamiento', fontname='DejaVu Sans', fontsize='12')
ax.set_xlabel('Tiempo', fontname='DejaVu Sans', fontsize='12')

#Cambiar ticks
#for label in ax.get_xticklabels():
    #label.set_fontproperties('Times New Roman')
#plt.xticks(fontsize='15')
#for label in ax.get_yticklabels():
    #label.set_fontproperties('Times New Roman')
#plt.yticks(fontsize='15')

# Creo los colores de más frío a más caliente
cmap = matplotlib.colormaps['nipy_spectral']
values = np.linspace(0.1, 0.9, 8)
colores = [cmap(value) for value in values]

# Creación de la gráfica
ax.plot(tiempo, data[0], linestyle='-', marker='', markersize=4, color=colores[0], label="T=1e-4", linewidth=1.0)
ax.plot(tiempo, data[1], linestyle='-', marker='', markersize=4, color=colores[1], label="T=1e-3", linewidth=1.0)
ax.plot(tiempo, data[2], linestyle='-', marker='', markersize=4, color=colores[2], label="T=1e-2", linewidth=1.0)
ax.plot(tiempo, data[3], linestyle='-', marker='', markersize=4, color=colores[3], label="T=2e-2", linewidth=1.0)
ax.plot(tiempo, data[4], linestyle='-', marker='', markersize=4, color=colores[4], label="T=3e-2", linewidth=1.0)
ax.plot(tiempo, data[5], linestyle='-', marker='', markersize=4, color=colores[5], label="T=4e-2", linewidth=1.0)
ax.plot(tiempo, data[6], linestyle='-', marker='', markersize=4, color=colores[6], label="T=5e-2", linewidth=1.0)
ax.plot(tiempo, data[7], linestyle='-', marker='', markersize=4, color=colores[7], label="T=1e-1", linewidth=1.0)


plt.legend(loc="upper right", fontsize=6)


# Guardar la gráfica
plt.savefig('grafica_solapamientos.pdf',dpi=300, bbox_inches = "tight")

# Mostrarla en pantalla
#plt.show()