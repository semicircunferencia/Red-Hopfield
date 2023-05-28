import matplotlib
import matplotlib.pyplot as plt
import numpy as np

plt.rcParams["font.family"] = "DejaVu Sans"
#fig = plt.figure(figsize=(11.,8.))      # figura (ancho, largo)
ax = plt.subplot()      # subfigura

# Datos
data=[0]*8
data1=[0]*8
data2=[0]*8
data3=[0]*8
for i in range(8):
    data[i]=np.loadtxt(f"T={i+1}.dat")
    data1[i]=abs(np.mean(data[i][70:101,0]))
    data2[i]=abs(np.mean(data[i][70:101,1]))
    data3[i]=abs(np.mean(data[i][70:101,2]))

temperatura = [1e-4, 1e-3, 1e-2, 2e-2, 3e-2, 4e-2, 5e-2, 1e-1]

# configurar ejes
ax.set_ylabel('Solapamiento', fontname='DejaVu Sans', fontsize='12')
ax.set_xlabel('Temperatura', fontname='DejaVu Sans', fontsize='12')

#Cambiar ticks
#for label in ax.get_xticklabels():
    #label.set_fontproperties('Times New Roman')
#plt.xticks(fontsize='15')
#for label in ax.get_yticklabels():
    #label.set_fontproperties('Times New Roman')
#plt.yticks(fontsize='15')

# Creación de la gráfica
ax.plot(temperatura, data1, linestyle='-', marker='.', markersize=4, color='#B4045F', label="XANA", linewidth=1.0)
ax.plot(temperatura, data2, linestyle='-', marker='.', markersize=4, color='#5FB404', label="Luna", linewidth=1.0)
ax.plot(temperatura, data3, linestyle='-', marker='.', markersize=4, color='#045FB4', label="Burbuja", linewidth=1.0)

plt.legend()


# Guardar la gráfica
plt.savefig('grafica_solapamientos.pdf',dpi=300, bbox_inches = "tight")

# Mostrarla en pantalla
#plt.show()