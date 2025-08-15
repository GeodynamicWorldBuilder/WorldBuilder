import numpy as np
import matplotlib.pyplot as plt
from scipy.special import erf

# --- 1. Parameters ---
L = 5.e5         # Depth of the domain (m)
N = 501          # Number of spatial points
dx = L / (N - 1) # Grid spacing
x = np.linspace(0, L, N)

# Physical properties (example)
alpha         = 3e-5    # 1/K
cp            = 1200    # Specific heat capacity (J/kg-K), assumed constant for simplicity
reference_rho = 3300
k0            = 4       # W/m/K
reference_T   = 293
gravity       = 10
T_surface     = 293.

# 'T-dependent' or 'constant'
formulation = 'T-dependent' 

# Time stepping
year_in_seconds = 3600*24*365.25
dt = 1e4 * year_in_seconds       # Time step (yr) -- must be small enough for stability
t_max = 2.5e8 * year_in_seconds    # Total time (yr)
n_steps = int(t_max / dt)

save_interval = int(n_steps / 250) # how often we store temperature, pressure, density

# Let's define a variable density and variable conductivity as functions of x
def density(T):
    if formulation == 'T-dependent':
        return reference_rho * (1. - alpha * (T - reference_T))
    elif formulation == 'constant':
        return reference_rho * np.ones(T.shape)

def conductivity(T, p):
    # Hofmeister
    if formulation == 'T-dependent':
        k = k0 * (298.0/T)**0.33 \
            * np.exp(-(4.0 * 1.2 + 1./3.) * alpha * (T - 298.0)) \
            * (1. + 4. * p / 1.35e11)  \
            + 0.01753 - 0.00010365 * T + 2.2451e-7 * T**2 - 3.407e-11 * T**3
    elif formulation == 'constant':
        k = k0 * np.ones(T.shape)
    return k

def integrate_pressure(rho_arr):
    # Here, for if rho depends on p, we would need to update rho as well
    p_arr = np.zeros(N)
    for i in range(1, N):
        p_arr[i] = p_arr[i-1] + rho_arr[i] * gravity * dx
    return p_arr


# --- 2. Initial Condition ---
T = np.zeros(N)

# Start with mantle potential temperature
T_mantle = 1623.0
T[:] = T_mantle
T[0] = T_surface

# --- 3. Boundary Conditions (Dirichlet) ---
# Use the initial conditions
T_top = T[0]
T_bottom = T[-1]

# Helper to get "interface conductivity" k_{i+1/2}
def k_interface(k_top, k_bot):
    # For simplicity, use average. Could also use harmonic mean, etc.
    return 0.5 * (k_top + k_bot)

# --- 4. Time-stepping loop (Explicit) ---

for step in range(n_steps):
    T_new = T.copy()

    # Evaluate properties at the current time
    rho_arr = density(T)

    # compute pressure
    p_arr  = integrate_pressure(rho_arr)
    k_arr   = conductivity(T, p_arr)

    if step == 0:
        # keep track of solutions for plotting
        T_history = [T.copy()]
        p_history   = [p_arr.copy()]  
        rho_history = [rho_arr.copy()]
        k_history   = [k_arr.copy()]

    # Enforce Dirichlet boundaries
    T_new[0] = T_top
    T_new[-1] = T_bottom

    # Update interior points
    for i in range(1, N-1):
        rho_i = rho_arr[i]

        # Compute effective conductivities at the interfaces
        k_imh = k_interface(k_arr[i-1], k_arr[i])   # k_{i-1/2}
        k_iph = k_interface(k_arr[i],   k_arr[i+1]) # k_{i+1/2}

        # Finite difference for second derivative with variable k
        d2T_dx2 = (k_iph*(T[i+1] - T[i]) - k_imh*(T[i] - T[i-1])) / (dx*dx)

        # Explicit update
        T_new[i] = T[i] + dt/(rho_i * cp) * d2T_dx2

    # Advance in time
    T = T_new

    # Store in history every so often
    # Do not store step 0 again
    # Note that technically, only T is from after solving, the other properties have not been updated
    if (step+1) % save_interval == 0:
        T_history.append(T.copy())
        p_history.append(p_arr.copy())  
        rho_history.append(rho_arr.copy())
        k_history.append(k_arr.copy())

# build time array for each saved snapshot
nt = len(T_history)  # number of snapshots actually stored
t = np.arange(nt) * (save_interval * dt)

# --- 5. Plot final solution ---

# T_2D is (nt, N), so we can do .T to get (N, nt)
Temperature_2D = np.array(T_history).T
pressure_2D = np.array(p_history).T
density_2D = np.array(rho_history).T
conductivity_2D = np.array(k_history).T

# Make coordinate grids
#  - "t" along the 'horizontal' direction (axis=1 or 0 depends on your preference)
#  - "x" along the 'vertical' direction

Xgrid, timegrid = np.meshgrid(x, t/(year_in_seconds * 1e6), indexing='ij')
# Now Xgrid and timegrid both have shape (N, nt)


plt.figure(figsize=(10,4))
# Make a "heatmap" of temperature. 
# Xgrid, timegrid, and Temperature_2D must be the same shape (N, nt).

plt.pcolormesh(timegrid, Xgrid/1000, Temperature_2D, shading='gouraud', edgecolors='face', linewidth=0)
plt.xlabel("Time [Myr]")
plt.ylabel("Depth [km]")
plt.gca().invert_yaxis()
cbar = plt.colorbar()
cbar.set_label("Temperature [K]")

plt.title("Temperature evolution")
plt.savefig('temperature.pdf', format='pdf')
plt.close()


plt.figure(figsize=(10,4))
plt.pcolormesh(timegrid, Xgrid/1000, pressure_2D - reference_rho * gravity * Xgrid, shading='gouraud', edgecolors='face', linewidth=0)
plt.xlabel("Time [Myr]")
plt.ylabel("Depth [km]")
plt.gca().invert_yaxis()
cbar = plt.colorbar()
cbar.set_label("Pressure [Pa]")

plt.title("Pressure difference from $rho_0 g z$")
plt.savefig('pressure.pdf', format='pdf')
plt.close()


plt.figure(figsize=(10,4))
plt.pcolormesh(timegrid, Xgrid/1000, density_2D, shading='gouraud', edgecolors='face', linewidth=0)
plt.xlabel("Time [Myr]")
plt.ylabel("Depth [km]")
plt.gca().invert_yaxis()
cbar = plt.colorbar()
cbar.set_label("Density [kg/m$^3$]")

plt.title("Density evolution")
plt.savefig('density.pdf', format='pdf')
plt.close()


plt.figure(figsize=(10,4))
plt.pcolormesh(timegrid, Xgrid/1000, conductivity_2D, shading='gouraud', edgecolors='face', linewidth=0)
plt.xlabel("Time [Myr]")
plt.ylabel("Depth [km]")
plt.gca().invert_yaxis()
cbar = plt.colorbar()
cbar.set_label("Conductivity [W/m/K]")

plt.title("Conductivity evolution")
plt.savefig('conductivity.pdf', format='pdf')
plt.close()

# Compare to erfc.
plt.figure(figsize=(10,4))
HSC_grid = T_surface + (T_mantle - T_surface) * erf(Xgrid/(2*np.sqrt(conductivity_2D*timegrid*year_in_seconds*1e6/(density_2D*cp))))
plt.pcolormesh(timegrid, Xgrid/1000, Temperature_2D - HSC_grid, shading='gouraud', edgecolors='face', linewidth=0)
plt.xlabel("Time [Myr]")
plt.ylabel("Depth [km]")
plt.gca().invert_yaxis()
cbar = plt.colorbar()
cbar.set_label("Temperature [K]")

plt.title("Temperature - error function")
plt.savefig('temperature_erfc.pdf', format='pdf')
plt.close()


# Write output into file
filename = "temperature_data.txt"

with open(filename, "w") as f:
    f.write("# Columns: time [yrs]   depth [m]   temperature\n")
    f.write("# POINTS: " + str(len(t)) + " " + str(len(x)) + "\n")
    for ix in range(len(x)):         # loop over time snapshots
        for it in range(len(t)):         # loop over the 50 x-values
            # Each line: time, depth, T
            # Right align each with chosen width and precision
            time_str = f"{t[it]/year_in_seconds:9.3e}"
            x_str    = f"{x[ix]:12.3e}"
            temp_str = f"{Temperature_2D[ix, it]:12.3f}"

            f.write(time_str + x_str + temp_str + "\n")

print(f"Data written to '{filename}'")
