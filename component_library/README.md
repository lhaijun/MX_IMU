# component_library

KiCad component library.

---

<details markdown="1">
  <summary>Table of Contents</summary>

<!-- TOC -->
* [component_library](#componentlibrary)
  * [1 Overview](#1-overview)
  * [2 Symbol Conventions](#2-symbol-conventions)
    * [2.1 General Properties](#21-general-properties)
    * [2.2 Simulation Model](#22-simulation-model)
  * [3 Footprint Conventions](#3-footprint-conventions)
    * [3.1 General Properties](#31-general-properties)
    * [3.2 Clearance Overrides and Settings](#32-clearance-overrides-and-settings)
    * [3.3 3D Models](#33-3d-models)
  * [4 Automations (GitHub Actions)](#4-automations-github-actions)
<!-- TOC -->

</details>

---

## 1 Overview

This repo stores all KiCad component related files (symbols, footprints, 3d
models, etc.). Conventions are listed below.

---

## 2 Symbol Conventions

### 2.1 General Properties

| n  | Name                       | Explained                    | Show | KiCad Default |
|----|----------------------------|------------------------------|------|---------------|
| 1  | `Reference`                | Symbol reference (REQUIRED)  | Yes  | Yes           |
| 2  | `Value`                    | Value, if applicable         | Yes  | Yes           |
| 3  | `Footprint`                | Location from repo root dir  | No   | Yes           |
| 4  | `Datasheet`                | URL/location of datasheet    | No   | Yes           |
| 5  | `Description`              | Short 1 sentence description | No   | Yes           |
| 6  | `Manufacturer`             | Fab/manufacturer name        | No   | No            |
| 7  | `Manufacturer Part Number` | Must match if added!         | No   | No            |
| 8  | `Distributor`              | Supplier/distributor name    | No   | No            |
| 9  | `Distributor Part Number`  | Must match if added!         | No   | No            |
| 10 | `Distributor Link`         | URL to product purchase page | No   | No            |

### 2.2 Simulation Model

Add simulation SPICE models for reasonable components as needed. Complete when
the benefit of an accurate SPICE model outweighs the required effort.

---

## 3 Footprint Conventions

### 3.1 General Properties

| n | Name                       | Explained                      | Show | Layer        | KiCad Default |
|---|----------------------------|--------------------------------|------|--------------|---------------|
| 1 | `Reference`                | Footprint reference (REQUIRED) | Yes  | F.Silkscreen | Yes           |
| 2 | `Value`                    | Value, if applicable           | Yes  | F.Fab        | Yes           |
| 3 | `Footprint`                | Location from repo root dir    | No   | No show      | Yes           |
| 4 | `Datasheet`                | URL/location of datasheet      | No   | No show      | Yes           |
| 5 | `Description`              | Short 1 sentence description   | No   | No show      | Yes           |
| 6 | `Manufacturer`             | Fab/manufacturer name          | No   | No show      | No            |
| 7 | `Manufacturer Part Number` | Must match if added!           | No   | No show      | No            |

### 3.2 Clearance Overrides and Settings

All optional, follow manufacturer recommendations.

### 3.3 3D Models

Always add a module unless there is none (PCB only footprint).

---

## 4 Automations (GitHub Actions)

GitHub actions pipelines are used to verify component information and supply
chain using distributor APIs.

The following distributors are currently supported:

1. DigiKey
2. Mouser

Distributor API supply chain checker workflows are saved
in [distributor_apis](docs/workflows/distributor_apis) for reference.

- Discontinued due to limited development and unnecessary slowdown in the
  current development process.

The badge markdown would be as follows:

```
![black_formatter](https://github.com/danielljeon/component_library/actions/workflows/black_formatter.yaml/badge.svg)
![distributor_apis](https://github.com/danielljeon/component_library/actions/workflows/distributor_apis.yaml/badge.svg)
```
